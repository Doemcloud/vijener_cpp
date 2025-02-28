#include <iostream>
#include <string>   

using namespace std;

class Vernam {
private:
    string k;

    string extendKey(string txt) {
        string extKey = k;
        while (extKey.length() < txt.length()) extKey += k;
        return extKey.substr(0, txt.length());
    }

public:
    Vernam(string key) : k(key) {}

    string encode(string text) {
        string extKey = extendKey(text);
        string result = "";

        for (int i = 0; i < text.length(); ++i) {
            char c = text[i] ^ extKey[i];
            result += c;
        }

        return result;
    }

    string decode(string text) {
        string extKey = extendKey(text);
        string result = "";

        for (int i = 0; i < text.length(); ++i) {
            char c = text[i] ^ extKey[i];
            result += c;
        }

        return result;
    }
};

int main() {
    string plaintext, key;

    cout << "Введите текст: ";
    getline(cin, plaintext);

    cout << "Введите ключ: ";
    getline(cin, key);

    Vernam vc(key);

    string encoded = vc.encode(plaintext);
    cout << "Зашифрованный текст: ";
    for (char c : encoded) {
        cout << "\\x" << hex << (unsigned int)(unsigned char)c;
    }
    cout << endl;

    string decoded = vc.decode(encoded);
    cout << "Расшифрованный текст: " << decoded << endl;

    return 0;
}