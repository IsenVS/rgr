#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <windows.h>
#include "helper.h"

using namespace std;

// �������� ������� ������
uint32_t LFSR[16];
// ������ �������� ���������
uint32_t FSM[3];

// ������� ��� ������������ FSM
void ClockFSM() {
    uint32_t F = (FSM[0] + FSM[1]) ^ FSM[2];
    FSM[0] = FSM[1];
    FSM[1] = FSM[2];
    FSM[2] = (F << 8) ^ (F >> 24); // ������ ���������� �������
}

// ������� ��� ������������ LFSR
void ClockLFSR() {
    uint32_t newLFSR = (LFSR[0] << 8) ^ (LFSR[1] >> 24) ^ LFSR[2] ^ (LFSR[3] >> 8);
    for (int i = 0; i < 15; i++) {
        LFSR[i] = LFSR[i + 1];
    }
    LFSR[15] = newLFSR;
}

// ������� ��������� ��������� ������
uint32_t GenerateKeystream() {
    ClockFSM();
    ClockLFSR();
    return LFSR[0] ^ FSM[2]; // ������ ��������� ��������� ������
}

// ������� ������������� � �������������� ����� � ������� �������������
void Initialize(uint32_t key[4], uint32_t iv[4]) {
    // ������������� LFSR � FSM � �������������� ����� � IV
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

    // ������������ �����
    for (int i = 0; i < 32; i++) {
        ClockFSM();
        ClockLFSR();
    }
}

// ������� ���������� ������
void Encrypt(vector<uint8_t>& data) {
    for (size_t i = 0; i < data.size(); i++) {
        uint32_t keystream = GenerateKeystream();
        data[i] ^= (keystream & 0xFF); // ���������� ������ �����
    }
}

// ������� ��� ������ �����
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
        cerr << "�� ������� ������� ���� " << filename << endl;
    }
}

// ������� ��� ������ ������ � ����
void writeFile(const string& filename, const vector<uint8_t>& data) {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();
    }
    else {
        cerr << "�� ������� ������� ���� " << filename << endl;
    }
}

// ������� ��� ������ ������������� ������ � ��������� � ����
void writeHexFileWithSpaces(const string& filename, const vector<uint8_t>& data) {
    ofstream file(filename);
    if (file.is_open()) {
        for (size_t i = 0; i < data.size(); i++) {
            if (i > 0) {
                file << " "; // ��������� ������ ����� �������
            }
            file << hex << setw(2) << setfill('0') << (int)data[i];
        }
        file.close();
    }
    else {
        cerr << "�� ������� ������� ���� " << filename << endl;
    }
}

// ������� ��� ������ ������������� ������ � ��������� �� �����
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
        cerr << "�� ������� ������� ���� " << filename << endl;
    }
}

// ������� ��� �������������� string � vector<uint8_t>
vector<uint8_t> stringToVector(const string& str) {
    return vector<uint8_t>(str.begin(), str.end());
}

void snow3GEncrypt() {

    // �������� ���������� ���� ����� �����������
    string secretCode = getSecretCode();
    if (secretCode != "12_03_ok") {
        cout << "������������ ��������� ���. �������� ��������." << endl;
        return;
    }

    // ����������� ������ ��� ����� � IV
    uint32_t key[4] = { 0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210 };
    uint32_t iv[4] = { 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff };

    int choice;
    cout << "�������� �������� ������:\n";
    cout << "1. ������ ����� �������\n";
    cout << "2. ������������ ����� �� ����� input_snow3g.txt\n";
    cout << "��� �����: ";
    cin >> choice;
    cin.ignore();  // ������� ������

    vector<uint8_t> data;
    if (choice == 1) {
        string message;
        cout << "������� ����� ��� ����������: ";
        getline(cin, message);
        data = stringToVector(message);
    }
    else if (choice == 2) {
        readFile("input_snow3g.txt", data);
        if (data.empty()) {
            cerr << "��������� �� ������� � �����: input_snow3g.txt" << endl;
            return;
        }
    }
    else {
        cout << "������������ �����.\n";
        return;
    }

    // ���������� ������
    Initialize(key, iv);
    Encrypt(data);
    writeHexFileWithSpaces("encrypted_snow3g.txt", data);
    cout << "������������� ��������� �������� � ����: encrypted_snow3g.txt\n";
    system("pause"); // ��� �������� ������� ������� ����� �������� �������
}

void snow3GDecrypt() {

    // �������� ���������� ���� ����� �������������
    string secretCode = getSecretCode();
    if (secretCode != "12_03_ok") {
        cout << "������������ ��������� ���. �������� ��������." << endl;
        return;
    }

    // ����������� ������ ��� ����� � IV
    uint32_t key[4] = { 0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210 };
    uint32_t iv[4] = { 0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff };

    // ������ �������������� ����� ��� ������������
    vector<uint8_t> encryptedData;
    readHexFileWithSpaces("encrypted_snow3g.txt", encryptedData);

    if (encryptedData.empty()) {
        cerr << "������������� ���� ���� ��� �� ����� ���� ��������" << endl;
        return;
    }

    // ������������ ������
    Initialize(key, iv); // ����������������� ��� ������������
    Encrypt(encryptedData);
    writeFile("decrypted_snow3g.txt", encryptedData);
    cout << "������������� ��������� �������� � ����: decrypted_snow3g.txt\n";
    system("pause"); // ��� �������� ������� ������� ����� �������� �������
}
