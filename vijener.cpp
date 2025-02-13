#include <iostream>
#include <string>
using namespace std;

class Cipher {
private:
    string alphabet;
    //Получение кода символа
    int keycode(char s) const {
        for (int i = 0; i < alphabet.length(); i++) {
            if (s == alphabet[i]) return i;
        }
        return 0;
    }

public:
    Cipher() : alphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZ") {}

    string Encode(const string& text, const string& key) const {
        string code;
        for (int i = 0; i < text.length(); i++) {
            code += alphabet[(keycode(text[i]) + keycode(key[i % key.length()])) % alphabet.length()];
        }
        return code;
    }

    string Decode(const string& text, const string& key) const {
        string code;
        for (int i = 0; i < text.length(); i++) {
            code += alphabet[(keycode(text[i]) - keycode(key[i % key.length()]) + alphabet.length()) % alphabet.length()];
        }
        return code;
    }
};

int main() {
    string txt, key;
    char x;

    Cipher cipher;

    cout << "Введите номер операции (1 - шифровка, 2 - дешифровка):" << endl;
    cin >> x;

    if (x != '1' && x != '2') {
        cout << "Неверное значение, выход из программы..." << endl;
        return 0;
    }

    cout << "Введите текст: ";
    cin >> txt;
    cout << "Введите ключ: ";
    cin >> key;
    for (auto& c : txt) c = toupper(c);
    for (auto& c : key) c = toupper(c);

    if (x == '1') {
        cout << "Зашифрованный текст: " << cipher.Encode(txt, key) << endl;
    } else if (x == '2') {
        cout << "Расшифрованный текст: " << cipher.Decode(txt, key) << endl;
    }

    return 0;
}