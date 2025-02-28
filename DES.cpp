#include <iostream>
#include <string>
#include <sys/_types/_u_int32_t.h>
#include <vector>
#include <bitset>

using namespace std;

class DES {
private:
    const int IP[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    };

    const int IP_INV[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
    };

    const int PC1[56] = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4
    };

    const int PC2[48] = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
    };

    const int E[48] = {
        32, 1, 2, 3, 4, 5,
        4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32, 1
    };

    const int P[32] = {
        16, 7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2, 8, 24, 14,
        32, 27, 3, 9,
        19, 13, 30, 6,
        22, 11, 4, 25
    };

    const int SHIFTS[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

    bitset<64> initialPermutation(bitset<64> data) {
        bitset<64> result;
        for (int i = 0; i < 64; i++) {
            result[i] = data[IP[i] - 1];
        }
        return result;
    }

    bitset<56> keyPermutation1(bitset<64> key) {
        bitset<56> result;
        for (int i = 0; i < 56; i++) {
            result[i] = key[PC1[i] - 1];
        }
        return result;
    }

    bitset<48> keyPermutation2(bitset<56> key) {
        bitset<48> result;
        for (int i = 0; i < 48; i++) {
            result[i] = key[PC2[i] - 1];
        }
        return result;
    }

    bitset<48> expansion(bitset<32> rightHalf) {
        bitset<48> result;
        for (int i = 0; i < 48; i++) {
            result[i] = rightHalf[E[i] - 1];
        }
        return result;
    }

    bitset<32> permutation(bitset<32> data) {
        bitset<32> result;
        for (int i = 0; i < 32; i++) {
            result[i] = data[P[i] - 1];
        }
        return result;
    }

    bitset<64> finalPermutation(bitset<64> data) {
        bitset<64> result;
        for (int i = 0; i < 64; i++) {
            result[i] = data[IP_INV[i] - 1];
        }
        return result;
    }

    bitset<28> rotateLeft(bitset<28> bits, int n) {
        string s = bits.to_string();
        return bitset<28>(s.substr(n) + s.substr(0, n));
    }

    bitset<32> fFunction(bitset<32> rightHalf, bitset<48> roundKey) {
        bitset<48> expanded = expansion(rightHalf);
        expanded ^= roundKey;
        return permutation(expanded.to_string().substr(0, 32));
    }

    vector<bitset<48>> generateRoundKeys(bitset<64> key) {
        bitset<56> permutedKey = keyPermutation1(key);
        bitset<28> leftHalf(permutedKey.to_string().substr(0, 28));
        bitset<28> rightHalf(permutedKey.to_string().substr(28, 28));
        vector<bitset<48>> roundKeys;
        for (int i = 0; i < 16; i++) {
            leftHalf = rotateLeft(leftHalf, SHIFTS[i]);
            rightHalf = rotateLeft(rightHalf, SHIFTS[i]);
            bitset<56> combined(leftHalf.to_string() + rightHalf.to_string());
            roundKeys.push_back(keyPermutation2(combined));
        }
        return roundKeys;
    }

public:
    bitset<64> encrypt(bitset<64> plaintext, bitset<64> key) {
        bitset<64> initialPermutedData = initialPermutation(plaintext);
        bitset<32> leftHalf(initialPermutedData.to_string().substr(0, 32));
        bitset<32> rightHalf(initialPermutedData.to_string().substr(32, 32));
        vector<bitset<48>> roundKeys = generateRoundKeys(key);
        for (int i = 0; i < 16; i++) {
            bitset<32> temp = rightHalf;
            rightHalf = leftHalf ^ fFunction(rightHalf, roundKeys[i]);
            leftHalf = temp;
        }
        bitset<64> combined(rightHalf.to_string() + leftHalf.to_string());
        return finalPermutation(combined);
    }
};

bitset<64> stringToBitset(string input) {
    string binaryStr;
    for (char c : input) {
        binaryStr += bitset<8>(c).to_string();
    }
    while (binaryStr.length() < 64) {
        binaryStr += "0";
    }
    return bitset<64>(binaryStr.substr(0, 64));
}

bitset<64> vectorToBitset(vector<int> input) {
    string binaryStr;
    for (int num : input) {
        binaryStr += bitset<4>(num).to_string();
    }
    while (binaryStr.length() < 64) {
        binaryStr = "0" + binaryStr;
    }
    return bitset<64>(binaryStr);
}

int main() {
    DES des;
    vector<int> ciphertext = {8, 2, 8, 11, 2, 0, 0, 5, 2, 11, 2, 8, 2, 5, 8, 11};
    string keyStr = "POTATO";
    bitset<64> cipherBits = vectorToBitset(ciphertext);
    bitset<64> keyBits = stringToBitset(keyStr);
    bitset<64> encrypted = des.encrypt(cipherBits, keyBits);
    cout << "Результат: " << encrypted << endl;
    return 0;
}