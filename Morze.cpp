#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <locale>
#include <codecvt>
#include <windows.h>
#include "helper.h"

using namespace std;

unordered_map<wchar_t, wstring> morse_en = {
    {L'A', L".-"}, {L'B', L"-..."}, {L'C', L"-.-."}, {L'D', L"-.."}, {L'E', L"."},
    {L'F', L"..-."}, {L'G', L"--."}, {L'H', L"...."}, {L'I', L".." }, {L'J', L".---"},
    {L'K', L"-.-"}, {L'L', L".-.."}, {L'M', L"--"}, {L'N', L"-."}, {L'O', L"---"},
    {L'P', L".--."}, {L'Q', L"--.-"}, {L'R', L".-."}, {L'S', L"..."}, {L'T', L"-"},
    {L'U', L"..-"}, {L'V', L"...-"}, {L'W', L".--"}, {L'X', L"-..-"}, {L'Y', L"-.--"},
    {L'Z', L"--.."}, {L'1', L".----"}, {L'2', L"..---"}, {L'3', L"...--"},
    {L'4', L"....-"}, {L'5', L"....."}, {L'6', L"-...."}, {L'7', L"--..."}, {L'8', L"---.."},
    {L'9', L"----."}, {L'0', L"-----"},
    {L'.', L".-.-.-"}, {L',', L"--..--"}, {L'?', L"..--.."}, {L'!', L"-.-.--"},
    {L'-', L"-....-"}, {L'/', L"-..-."}, {L'@', L".--.-."}, {L'(', L"-.--."}, {L')', L"-.--.-"},
    {L':', L"---..."}
};

unordered_map<wchar_t, wstring> morse_ru = {
    {L'А', L".-"}, {L'Б', L"-..."}, {L'В', L".--"}, {L'Г', L"--."}, {L'Д', L"-.."},
    {L'Е', L"."}, {L'Ё', L"."}, {L'Ж', L"...-"}, {L'З', L"--.."}, {L'И', L".." }, {L'Й', L".---"},
    {L'К', L"-.-"}, {L'Л', L".-.."}, {L'М', L"--"}, {L'Н', L"-."}, {L'О', L"---"},
    {L'П', L".--."}, {L'Р', L".-."}, {L'С', L"..."}, {L'Т', L"-"}, {L'У', L"..-"},
    {L'Ф', L"..-."}, {L'Х', L"...."}, {L'Ц', L"-.-."}, {L'Ч', L"---."}, {L'Ш', L"----"},
    {L'Щ', L"--.-"}, {L'Ъ', L".--.-."}, {L'Ы', L"-.--"}, {L'Ь', L"-..-"},
    {L'Э', L"..-."}, {L'Ю', L"..--"}, {L'Я', L".-.-"},
    {L'1', L".----"}, {L'2', L"..---"}, {L'3', L"...--"}, {L'4', L"....-"}, {L'5', L"....."},
    {L'6', L"-...."}, {L'7', L"--..."}, {L'8', L"---.."}, {L'9', L"----."},
    {L'0', L"-----"},
    {L'.', L".-.-.-"}, {L',', L"--..--"}, {L'?', L"..--.."}, {L'!', L"-.-.--"},
    {L'-', L"-....-"}, {L'/', L"-..-."}, {L'@', L".--.-."}, {L'(', L"-.--."}, {L')', L"-.--.-"},
    {L':', L"---..."}
};

bool isRussian(const wstring& text) {
    for (wchar_t ch : text) {
        if ((ch >= L'А' && ch <= L'Я') || ch == L'Ё') {
            return true;
        }
    }
    return false;
}

wstring toUpperCase(const wstring& text) {
    wstring upper_text;
    for (wchar_t ch : text) {
        upper_text += towupper(ch);
    }
    return upper_text;
}

wstring encrypt(const wstring& text, const unordered_map<wchar_t, wstring>& morse) {
    wstring encrypted;
    for (wchar_t ch : text) {
        if (ch == L'\n') {
            encrypted += L"\n";
        }
        else if (ch == L' ') {
            encrypted += L" / ";
        }
        else {
            auto it = morse.find(ch);
            if (it != morse.end()) {
                encrypted += it->second + L" ";
            }
        }
    }
    return encrypted;
}

wstring decrypt(const wstring& morse_text, const unordered_map<wstring, wchar_t>& reverse_morse) {
    wstring decrypted;
    wstring morse_char;
    for (wchar_t ch : morse_text) {
        if (ch == L' ') {
            if (!morse_char.empty()) {
                auto it = reverse_morse.find(morse_char);
                if (it != reverse_morse.end()) {
                    decrypted += it->second;
                }
                morse_char.clear();
            }
        }
        else if (ch == L'/') {
            decrypted += L' ';
        }
        else if (ch == L'\n') {
            decrypted += L'\n';
        }
        else {
            morse_char += ch;
        }
    }
    if (!morse_char.empty()) {
        auto it = reverse_morse.find(morse_char);
        if (it != reverse_morse.end()) {
            decrypted += it->second;
        }
    }
    return decrypted;
}

unordered_map<wstring, wchar_t> createReverseMorse(const unordered_map<wchar_t, wstring>& morse) {
    unordered_map<wstring, wchar_t> reverse_morse;
    for (const auto& pair : morse) {
        reverse_morse[pair.second] = pair.first;
    }
    return reverse_morse;
}

void morseEncrypt() {

    // Проверка секретного кода перед шифрованием
    string secretCode = getSecretCode();
    if (secretCode != "12_03_ok") {
        cout << "Неправильный секретный код. Операция отменена." << endl;
        return;
    }

    int languageChoice;
    cout << "Выберите язык:\n";
    cout << "1. Английский\n";
    cout << "2. Русский\n";
    cout << "Ваш выбор: ";
    cin >> languageChoice;
    wcin.ignore();  // Очистка буфера

    wstring input_text;
    wifstream input_file(languageChoice == 1 ? "input_en.txt" : "input_ru.txt");
    input_file.imbue(locale(input_file.getloc(), new codecvt_utf8<wchar_t>));
    input_text = wstring((istreambuf_iterator<wchar_t>(input_file)), istreambuf_iterator<wchar_t>());

    input_text = toUpperCase(input_text);
    const auto& morse = (languageChoice == 1) ? morse_en : morse_ru;

    wstring encrypted_text = encrypt(input_text, morse);

    wofstream encrypted_file("encrypted_morse.txt");
    encrypted_file.imbue(locale(encrypted_file.getloc(), new codecvt_utf8<wchar_t>));
    encrypted_file << encrypted_text << endl;

    cout << "Текст успешно зашифрован и записан в файл encrypted_morse.txt\n";
    system("pause"); // Для ожидания нажатия клавиши перед очисткой консоли
}

void morseDecrypt() {

    // Проверка секретного кода перед дешифрованием
    string secretCode = getSecretCode();
    if (secretCode != "12_03_ok") {
        cout << "Неправильный секретный код. Операция отменена." << endl;
        return;
    }

    int languageChoice;
    cout << "Выберите язык:\n";
    cout << "1. Английский\n";
    cout << "2. Русский\n";
    cout << "Ваш выбор: ";
    cin >> languageChoice;
    wcin.ignore();  // Очистка буфера

    wstring morse_text;
    wifstream input_file("encrypted_morse.txt");
    input_file.imbue(locale(input_file.getloc(), new codecvt_utf8<wchar_t>));
    morse_text = wstring((istreambuf_iterator<wchar_t>(input_file)), istreambuf_iterator<wchar_t>());

    const auto& morse = (languageChoice == 1) ? morse_en : morse_ru;
    const auto reverse_morse = createReverseMorse(morse);

    wstring decrypted_text = decrypt(morse_text, reverse_morse);

    wofstream decrypted_file("decrypted_morse.txt");
    decrypted_file.imbue(locale(decrypted_file.getloc(), new codecvt_utf8<wchar_t>));
    decrypted_file << decrypted_text << endl;

    cout << "Текст успешно дешифрован и записан в файл decrypted_morse.txt\n";
    system("pause"); // Для ожидания нажатия клавиши перед очисткой консоли
}
