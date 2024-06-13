#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <windows.h>
#include "helper.h"

using namespace std;

// Линейный регистр сдвига
uint32_t LFSR[16];
// Машина конечных состояний
uint32_t FSM[3];

// Функция для тактирования FSM
void ClockFSM() {
    uint32_t F = (FSM[0] + FSM[1]) ^ FSM[2];
    FSM[0] = FSM[1];
    FSM[1] = FSM[2];
    FSM[2] = (F << 8) ^ (F >> 24); // Пример нелинейной функции
}

// Функция для тактирования LFSR
void ClockLFSR() {
    uint32_t newLFSR = (LFSR[0] << 8) ^ (LFSR[1] >> 24) ^ LFSR[2] ^ (LFSR[3] >> 8);
    for (int i = 0; i < 15; i++) {
        LFSR[i] = LFSR[i + 1];
    }
    LFSR[15] = newLFSR;
}

// Функция генерации ключевого потока
uint32_t GenerateKeystream() {
    ClockFSM();
    ClockLFSR();
    return LFSR[0] ^ FSM[2]; // Пример генерации ключевого потока
}

// Функция инициализации с использованием ключа и вектора инициализации
void Initialize(uint32_t key[4], uint32_t iv[4]) {
    // Инициализация LFSR и FSM с использованием ключа и IV
    for (int i = 0; i < 4; i++) {
        LFSR[i] = key[i];
        LFSR[i + 4] = iv[i];
    }
    for (int i = 8; i < 16; i++) {
        LFSR[i] = LFSR[i - 8] ^ LFSR[i - 4];
    }
    FSM[0] = 0;
    FSM[1] = 0;
    FSM[2] = 0;

    // Прогревочные такты
    for (int i = 0; i < 32; i++) {
        ClockFSM();
        ClockLFSR();
    }
}

// Функция шифрования данных
void Encrypt(vector<uint8_t>& data) {
    for (size_t i = 0; i < data.size(); i++) {
        uint32_t keystream = GenerateKeystream();
        data[i] ^= (keystream & 0xFF); // Шифрование одного байта
    }
}

// Функция для чтения файла
void readFile(const string& filename, vector<uint8_t>& data) {
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        file.seekg(0, ios::end);
        size_t size = file.tellg();
        file.seekg(0, ios::beg);
        data.resize(size);
        file.read(reinterpret_cast<char*>(data.data()), size);
        file.close();
    }
    else {
        cerr << "Не удалось открыть файл " << filename << endl;
    }
}

// Функция для записи данных в файл
void writeFile(const string& filename, const vector<uint8_t>& data) {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();
    }
    else {
        cerr << "Не удалось открыть файл " << filename << endl;
    }
}

// Функция для записи зашифрованных данных с пробелами в файл
void writeHexFileWithSpaces(const string& filename, const vector<uint8_t>& data) {
    ofstream file(filename);
    if (file.is_open()) {
        for (size_t i = 0; i < data.size(); i++) {
            if (i > 0) {
                file << " "; // Добавляем пробел между байтами
            }
            file << hex << setw(2) << setfill('0') << (int)data[i];
        }
        file.close();
    }
    else {
        cerr << "Не удалось открыть файл " << filename << endl;
    }
}

// Функция для чтения зашифрованных данных с пробелами из файла
void readHexFileWithSpaces(const string& filename, vector<uint8_t>& data) {
    ifstream file(filename);
    if (file.is_open()) {
        string hexData;
        getline(file, hexData);
        data.clear();
        istringstream ss(hexData);
        string byteString;
        while (ss >> byteString) {
            uint8_t byte = (uint8_t)strtol(byteString.c_str(), nullptr, 16);
            data.push_back(byte);
        }
        file.close();
    }
    else {
        cerr << "Не удалось открыть файл " << filename << endl;
    }
}

// Функция для преобразования string в vector<uint8_t>
vector<uint8_t> stringToVector(const string& str) {
    return vector<uint8_t>(str.begin(), str.end());
}

void snow3GEncrypt() {

    // Проверка секретного кода перед шифрованием
    string secretCode = getSecretCode();
    if (secretCode != "12_03_ok") {
        cout << "Неправильный секретный код. Операция отменена." << endl;
        return;
    }

    // Стандартные данные для ключа и IV
    uint32_t key[4] = { 0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210 };
    uint32_t iv[4] = { 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff };

    int choice;
    cout << "Выберите источник текста:\n";
    cout << "1. Ввести текст вручную\n";
    cout << "2. Использовать текст из файла input_snow3g.txt\n";
    cout << "Ваш выбор: ";
    cin >> choice;
    cin.ignore();  // Очистка буфера

    vector<uint8_t> data;
    if (choice == 1) {
        string message;
        cout << "Введите текст для шифрования: ";
        getline(cin, message);
        data = stringToVector(message);
    }
    else if (choice == 2) {
        readFile("input_snow3g.txt", data);
        if (data.empty()) {
            cerr << "Сообщение не найдено в файле: input_snow3g.txt" << endl;
            return;
        }
    }
    else {
        cout << "Неправильный выбор.\n";
        return;
    }

    // Шифрование данных
    Initialize(key, iv);
    Encrypt(data);
    writeHexFileWithSpaces("encrypted_snow3g.txt", data);
    cout << "Зашифрованное сообщение записано в файл: encrypted_snow3g.txt\n";
    system("pause"); // Для ожидания нажатия клавиши перед очисткой консоли
}

void snow3GDecrypt() {

    // Проверка секретного кода перед дешифрованием
    string secretCode = getSecretCode();
    if (secretCode != "12_03_ok") {
        cout << "Неправильный секретный код. Операция отменена." << endl;
        return;
    }

    // Стандартные данные для ключа и IV
    uint32_t key[4] = { 0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210 };
    uint32_t iv[4] = { 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff };

    // Чтение зашифрованного файла для дешифрования
    vector<uint8_t> encryptedData;
    readHexFileWithSpaces("encrypted_snow3g.txt", encryptedData);

    if (encryptedData.empty()) {
        cerr << "Зашифрованный файл пуст или не может быть прочитан" << endl;
        return;
    }

    // Дешифрование данных
    Initialize(key, iv); // Переинициализация для дешифрования
    Encrypt(encryptedData);
    writeFile("decrypted_snow3g.txt", encryptedData);
    cout << "Дешифрованное сообщение записано в файл: decrypted_snow3g.txt\n";
    system("pause"); // Для ожидания нажатия клавиши перед очисткой консоли
}
