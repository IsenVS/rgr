#ifndef HELPER_H
#define HELPER_H

#include <string>

// Прототипы функций
void showEncryptDecryptMenu(const std::string& cipher_name);
void chooseCipher();
void morseEncrypt();
void morseDecrypt();
void linelSenSiraEncrypt();
void linelSenSiraDecrypt();
void snow3GEncrypt();
void snow3GDecrypt();
void setConsoleEncoding();
std::string getSecretCode();

extern void clearConsole(); // Объявление функции с extern

#endif // HELPER_H
