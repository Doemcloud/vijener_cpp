#include <iostream>
#include <cstdint>
#include <vector>
#include <cstring>

using namespace std;

class Speck {
private:
    uint32_t alpha = 3;
    uint32_t beta = 8;
    uint32_t rounds = 32;

    uint64_t rotate_left(uint64_t x, uint32_t n) {
        return (x << n) | (x >> (64 - n));
    }

    uint64_t rotate_right(uint64_t x, uint32_t n) {
        return (x >> n) | (x << (64 - n));
    }

    void speck_round(uint64_t &x, uint64_t &y, uint64_t k) {
        x = rotate_right(x, beta);
        x += y;
        x ^= k;
        y = rotate_left(y, alpha);
        y ^= x;
    }

    void speck_inv_round(uint64_t &x, uint64_t &y, uint64_t k) {
        y ^= x;
        y = rotate_right(y, alpha);
        x ^= k;
        x -= y;
        x = rotate_left(x, beta);
    }

public:
    vector<uint64_t> key_schedule(uint64_t key) {
        vector<uint64_t> round_keys(rounds);
        uint64_t l = key >> 64;
        uint64_t k = key & 0xFFFFFFFFFFFFFFFF;

        round_keys[0] = k;
        for (uint32_t i = 0; i < rounds - 1; ++i) {
            l = rotate_left(l, alpha);
            l ^= k;
            k = rotate_right(k, beta);
            k ^= (i + 1);
            round_keys[i + 1] = k;
        }
        return round_keys;
    }

    vector<uint64_t> encrypt(const vector<uint64_t> &plaintext, const vector<uint64_t> &round_keys) {
        vector<uint64_t> ciphertext = plaintext;
        for (uint32_t i = 0; i < rounds; ++i) {
            speck_round(ciphertext[0], ciphertext[1], round_keys[i]);
        }
        return ciphertext;
    }

    vector<uint64_t> decrypt(const vector<uint64_t> &ciphertext, const vector<uint64_t> &round_keys) {
        vector<uint64_t> plaintext = ciphertext;
        for (int i = rounds - 1; i >= 0; --i) {
            speck_inv_round(plaintext[0], plaintext[1], round_keys[i]);
        }
        return plaintext;
    }
};

class CBC {
private:
    Speck speck;

public:
    vector<uint64_t> encrypt(const vector<uint64_t> &plaintext, const vector<uint64_t> &round_keys, uint64_t iv) {
        vector<uint64_t> ciphertext;
        uint64_t previous_block = iv;

        for (size_t i = 0; i < plaintext.size(); i += 2) {
            uint64_t block1 = plaintext[i] ^ previous_block;
            uint64_t block2 = plaintext[i + 1];
            vector<uint64_t> encrypted_block = speck.encrypt({block1, block2}, round_keys);
            ciphertext.push_back(encrypted_block[0]);
            ciphertext.push_back(encrypted_block[1]);
            previous_block = encrypted_block[0];
        }
        return ciphertext;
    }

    vector<uint64_t> decrypt(const vector<uint64_t> &ciphertext, const vector<uint64_t> &round_keys, uint64_t iv) {
        vector<uint64_t> plaintext;
        uint64_t previous_block = iv;

        for (size_t i = 0; i < ciphertext.size(); i += 2) {
            vector<uint64_t> decrypted_block = speck.decrypt({ciphertext[i], ciphertext[i + 1]}, round_keys);
            uint64_t block1 = decrypted_block[0] ^ previous_block;
            uint64_t block2 = decrypted_block[1];
            plaintext.push_back(block1);
            plaintext.push_back(block2);
            previous_block = ciphertext[i];
        }
        return plaintext;
    }
};

class MDC2 {
private:
    Speck speck;
    uint64_t g0 = 0x5252525252525252;
    uint64_t h0 = 0x2525252525252525;

public:
    vector<uint64_t> hash(const vector<uint64_t> &message, const vector<uint64_t> &round_keys) {
        uint64_t g = g0;
        uint64_t h = h0;

        for (size_t i = 0; i < message.size(); i += 2) {
            vector<uint64_t> block = {message[i], message[i + 1]};
            vector<uint64_t> encrypted_block = speck.encrypt(block, round_keys);

            g ^= encrypted_block[0];
            h ^= encrypted_block[1];
        }

        return {g, h};
    }
    bool checkhash(const vector<uint64_t> &message,
                const vector<uint64_t> &round_keys,
                const vector<uint64_t> &expected_hash) {

        if (expected_hash.size() != 2) {
            return false;
        }
        vector<uint64_t> calculated_hash = hash(message, round_keys);

        return (calculated_hash[0] == expected_hash[0] &&
                calculated_hash[1] == expected_hash[1]);
    }
};

class CSPRNG {
private:
    Speck speck;
    vector<uint64_t> round_keys;
    uint64_t state;

public:
    CSPRNG(uint64_t seed) {
        round_keys = speck.key_schedule(seed);
        state = seed;
    }

    uint64_t generate() {
        vector<uint64_t> block = {state, state};
        vector<uint64_t> encrypted_block = speck.encrypt(block, round_keys);
        state = encrypted_block[0];
        return state;
    }
};

int main() {
    uint64_t key = 0xFEDCBA9876543;
    uint64_t iv = 0xFEDCBA9876543;

    Speck speck;
    CBC cbc;
    MDC2 mdc2;

    vector<uint64_t> plaintext = {0x0123456789ABCDEF, 0x0123456789ABCDEF};
    vector<uint64_t> round_keys = speck.key_schedule(key);

    vector<uint64_t> ciphertext = cbc.encrypt(plaintext, round_keys, iv);
    cout << "Введите текст: " << hex << ciphertext[0] << ", " << ciphertext[1] << endl;

    vector<uint64_t> decrypted = cbc.decrypt(ciphertext, round_keys, iv);
    cout << "Decrypted: " << hex << decrypted[0] << ", " << decrypted[1] << endl;

    vector<uint64_t> hash = mdc2.hash(plaintext, round_keys);
    cout << "Hash: " << hex << hash[0] << ", " << hash[1] << endl;

    bool hash_verified = mdc2.checkhash(plaintext, round_keys, hash);
    cerr << "Hash check: " << (hash_verified ? "TRUE" : "FALSE") << endl;

    CSPRNG csprng(key);
    cout << "Random number: " << hex << csprng.generate() << endl;

    return 0;
}