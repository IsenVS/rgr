#include "helper.h"
#include <iostream>
#include <locale>
#include <string>
#include <windows.h>

using namespace std;

void setConsoleEncoding() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
}

string getSecretCode() {
    string secretCode;
    cout << "������� ��������� ���: ";
    cin >> secretCode;
    return secretCode;
}

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void showEncryptDecryptMenu(const string& cipher_name) {
    while (true) {
        clearConsole();
        cout << "�������� ��������:\n";
        cout << "1. ����������\n";
        cout << "2. ������������\n";
        cout << "3. ��������� �����\n";
        cout << "��� �����: ";

        int choice;
        cin >> choice;
        cin.ignore();  // ������� ������

        if (cipher_name == "Morse") {
            switch (choice) {
            case 1:
                morseEncrypt();
                break;
            case 2:
                morseDecrypt();
                break;
            case 3:
                return;
            default:
                cout << "������������ �����, ���������� ��� ���.\n";
            }
        }
        else if (cipher_name == "������� ���-����") {
            switch (choice) {
            case 1:
                linelSenSiraEncrypt();
                break;
            case 2:
                linelSenSiraDecrypt();
                break;
            case 3:
                return;
            default:
                cout << "������������ �����, ���������� ��� ���.\n";
            }
        }
        else if (cipher_name == "Snow3G") {
            switch (choice) {
            case 1:
                snow3GEncrypt();
                break;
            case 2:
                snow3GDecrypt();
                break;
            case 3:
                return;
            default:
                cout << "������������ �����, ���������� ��� ���.\n";
            }
        }
    }
}

void chooseCipher() {
    while (true) {
        clearConsole();
        cout << "�������� ����:\n";
        cout << "1. ���� �����\n";
        cout << "2. ������� ���-����\n";
        cout << "3. Snow3G\n";
        cout << "4. �����\n";
        cout << "��� �����: ";

        int choice;
        cin >> choice;
        cin.ignore();  // ������� ������

        switch (choice) {
        case 1:
            showEncryptDecryptMenu("Morse");
            break;
        case 2:
            showEncryptDecryptMenu("������� ���-����");
            break;
        case 3:
            showEncryptDecryptMenu("Snow3G");
            break;
        case 4:
            cout << "����� �� ���������.\n";
            return;
        default:
            cout << "������������ �����, ���������� ��� ���.\n";
            system("pause");
        }
    }
}
