#include <iostream>
#include <string>

using namespace std;

string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

//Код символа
int keycode(char s){
    for(int i = 0; i < alphabet.length(); i++){
        if(s == alphabet[i]) return i;
    }
    return 0;
}

string Encode(string text, string key){
    string code;
    for(int i = 0; i < text.length(); i++){
        code += alphabet[(keycode(text[i]) + keycode(key[i % key.length()])) % alphabet.length()];
    }
    return code;
}

string Decode(string text, string key){
    string code;
    for(int i = 0; i < text.length(); i++){
        code += alphabet[(keycode(text[i]) - keycode(key[i % key.length()]) + alphabet.length()) % alphabet.length()];
    }
    return code;
}

int main(){
    string txt, key;
    char x;
    cout << "Введите номер операции (1 - шивфровка, 2 - дешифровка)" << endl;
    cin >> x;

    if(x != '1' && x != '2'){
        cout << "Неверное значение, выход из программы..." << endl;
        exit(0);
    }

    cout << "введите текст: ";
    cin >> txt;
    cout << "введите ключ: ";
    cin >> key;

    for(auto& c : txt) c = toupper(c);
    for(auto& c : key) c = toupper(c);

    if(x == '1') cout << Encode(txt, key);
    if(x == '2') cout << Decode(txt, key);
}