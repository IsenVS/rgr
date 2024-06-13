#ifndef HELPER_H
#define HELPER_H

#include <string>

// ��������� �������
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

extern void clearConsole(); // ���������� ������� � extern

#endif // HELPER_H
