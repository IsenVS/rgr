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
    cout << "Введите секретный код: ";
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
        cout << "Выберите действие:\n";
        cout << "1. Шифрование\n";
        cout << "2. Дешифрование\n";
        cout << "3. Вернуться назад\n";
        cout << "Ваш выбор: ";

        int choice;
        cin >> choice;
        cin.ignore();  // Очистка буфера

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
                cout << "Неправильный выбор, попробуйте еще раз.\n";
            }
        }
        else if (cipher_name == "Линейка Сен-Сира") {
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
                cout << "Неправильный выбор, попробуйте еще раз.\n";
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
                cout << "Неправильный выбор, попробуйте еще раз.\n";
            }
        }
    }
}

void chooseCipher() {
    while (true) {
        clearConsole();
        cout << "Выберите шифр:\n";
        cout << "1. Шифр Морзе\n";
        cout << "2. Линейка Сен-Сира\n";
        cout << "3. Snow3G\n";
        cout << "4. Выйти\n";
        cout << "Ваш выбор: ";

        int choice;
        cin >> choice;
        cin.ignore();  // Очистка буфера

        switch (choice) {
        case 1:
            showEncryptDecryptMenu("Morse");
            break;
        case 2:
            showEncryptDecryptMenu("Линейка Сен-Сира");
            break;
        case 3:
            showEncryptDecryptMenu("Snow3G");
            break;
        case 4:
            cout << "Выход из программы.\n";
            return;
        default:
            cout << "Неправильный выбор, попробуйте еще раз.\n";
            system("pause");
        }
    }
}
