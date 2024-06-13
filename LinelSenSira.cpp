#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <locale>
#include <codecvt>
#include "helper.h"

using namespace std;

// Константы для алфавита и двойного алфавита
const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя0123456789 !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
const string DOUBLE_ALPHABET = ALPHABET + ALPHABET;

// Функция для расширения ключа до длины сообщения
string extendKey(const string& message, const string& key) {
    string extendedKey = key;
    while (extendedKey.size() < message.size()) {
        extendedKey += key;
    }
    return extendedKey.substr(0, message.size());
}

// Функция для шифрования одного символа
char encryptChar(char messageChar, char keyChar) {
    int messageIndex = ALPHABET.find(messageChar);
    int keyIndex = ALPHABET.find(keyChar);
    if (messageIndex == string::npos || keyIndex == string::npos) {
        return messageChar; // возвращаем символ без изменений, если он не найден в алфавите
    }
    return DOUBLE_ALPHABET[keyIndex + messageIndex];
}

// Функция для дешифрования одного символа
char decryptChar(char encryptedChar, char keyChar) {
    int encryptedIndex = ALPHABET.find(encryptedChar);
    int keyIndex = ALPHABET.find(keyChar);
    if (encryptedIndex == string::npos || keyIndex == string::npos) {
        return encryptedChar; // возвращаем символ без изменений, если он не найден в алфавите
    }
    return DOUBLE_ALPHABET[encryptedIndex + ALPHABET.size() - keyIndex];
}

// Функция для обработки сообщения (шифрование или дешифрование)
string processMessage(const string& message, const string& key, bool encrypt) {
    string extendedKey = extendKey(message, key);
    string processedMessage = "";

    for (size_t i = 0; i < message.size(); ++i) {
        if (encrypt) {
            processedMessage += encryptChar(message[i], extendedKey[i]);
        }
        else {
            processedMessage += decryptChar(message[i], extendedKey[i]);
        }
    }

    return processedMessage;
}

// Функция для записи строки в файл
void writeToFile(const string& filename, const string& content) {
    ofstream outFile(filename, ios::binary);
    if (outFile.is_open()) {
        outFile.write(content.c_str(), content.size());
        outFile.close();
    }
    else {
        cerr << "Невозможно открыть файл: " << filename << endl;
    }
}

// Функция для чтения строки из файла
string readFromFile(const string& filename) {
    ifstream inFile(filename, ios::binary);
    string content = "";

    if (inFile.is_open()) {
        inFile.seekg(0, ios::end);
        content.resize(inFile.tellg());
        inFile.seekg(0, ios::beg);
        inFile.read(&content[0], content.size());
        inFile.close();
    }
    else {
        cerr << "Невозможно открыть файл: " << filename << endl;
    }

    return content;
}

// Функция для преобразования из wstring в string с кодировкой Windows-1251
string wstringToString(const wstring& wstr) {
    int bufferSize = WideCharToMultiByte(1251, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    string str(bufferSize, 0);
    WideCharToMultiByte(1251, 0, wstr.c_str(), -1, &str[0], bufferSize, nullptr, nullptr);
    return str;
}

// Функция для преобразования из string с кодировкой Windows-1251 в wstring
wstring stringToWstring(const string& str) {
    int bufferSize = MultiByteToWideChar(1251, 0, str.c_str(), -1, nullptr, 0);
    wstring wstr(bufferSize, 0);
    MultiByteToWideChar(1251, 0, str.c_str(), -1, &wstr[0], bufferSize);
    return wstr;
}

void linelSenSiraEncrypt() {

    // Проверка секретного кода перед шифрованием
    string secretCode = getSecretCode();
    if (secretCode != "12_03_ok") {
        cout << "Неправильный секретный код. Операция отменена." << endl;
        return;
    }

   

    int choice;
    cout << "Выберите источник текста:\n";
    cout << "1. Ввести текст вручную\n";
    cout << "2. Использовать текст из файла input_sen-siro.txt\n";
    cout << "Ваш выбор: ";
    cin >> choice;
    cin.ignore();  // Очистка буфера
    wstring wkey;
    cout << "Введите ключ: "; // Запрос ключа у пользователя
    getline(wcin, wkey);
    string key = wstringToString(wkey);

    string message;
    if (choice == 1) {
        wstring wmessage;
        cout << "Введите текст для шифрования: ";
        getline(wcin, wmessage);
        message = wstringToString(wmessage);
    }
    else if (choice == 2) {
        string fileContent = readFromFile("input_sen-siro.txt");
        message = fileContent;
        if (message.empty()) {
            cerr << "Сообщение не найдено в файле: input_sen-siro.txt" << endl;
            return;
        }
    }
    else {
        cout << "Неправильный выбор.\n";
        return;
    }

    // Шифрование сообщения
    string encryptedMessage = processMessage(message, key, true);
    writeToFile("encrypted_linel.txt", encryptedMessage);
    cout << "Зашифрованное сообщение записано в файл: encrypted_linel.txt\n";
    system("pause"); // Для ожидания нажатия клавиши перед очисткой консоли
}

void linelSenSiraDecrypt() {

    // Проверка секретного кода перед дешифрованием
    string secretCode = getSecretCode();
    if (secretCode != "12_03_ok") {
        cout << "Неправильный секретный код. Операция отменена." << endl;
        return;
    }
    cin.ignore();
    wstring wkey;
    cout << "Введите ключ: "; // Запрос ключа у пользователя
    getline(wcin, wkey);
    string key = wstringToString(wkey);

    string encryptedContent = readFromFile("encrypted_linel.txt");
    if (encryptedContent.empty()) {
        cerr << "Сообщение не найдено в файле: encrypted_linel.txt" << endl;
        return;
    }


    // Дешифрование сообщения
    string decryptedMessage = processMessage(encryptedContent, key, false);
    writeToFile("decrypted_linel.txt", decryptedMessage);
    cout << "Дешифрованное сообщение записано в файл: decrypted_linel.txt\n";
    system("pause"); // Для ожидания нажатия клавиши перед очисткой консоли
}

