#include "helper.h"
#include <locale>
#include <iostream>
#include <windows.h>

using namespace std;

bool checkPassword() {
    const string correctPassword = "Dbrnjh"; // Задайте здесь ваш пароль
    string inputPassword;
    cout << "Введите пароль: ";
    cin >> inputPassword;
    if (inputPassword == correctPassword) {
        return true;
    }
    else {
        cout << "Неправильный пароль. Программа завершает работу." << endl;
        return false;
    }
}

int main() {
    setConsoleEncoding();
    setlocale(LC_ALL, "ru");

    if (checkPassword()) {
        chooseCipher();
    }

    return 0;
}
