#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <windows.h>

using namespace std;

const uint8_t SBOX[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0xe0, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const uint32_t RCON[10] = {
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 
    0x10000000, 0x20000000, 0x40000000, 0x80000000, 
    0x1B000000, 0x36000000
};

struct CryptoParams {
    vector<uint8_t> key;
    vector<uint8_t> iv;
};

uint8_t galois_mul2(uint8_t value) {
    return (value & 0x80) ? (value << 1) ^ 0x1B : (value << 1);
}

uint8_t galois_mul3(uint8_t value) {
    return galois_mul2(value) ^ value;
}

void printState(const string& stage, const vector<uint8_t>& state) {
    cout << stage << ": ";
    for (uint8_t b : state) {
        cout << hex << setw(2) << setfill('0') << (int)b << " ";
    }
    cout << dec << endl;
}

void addRoundKey(vector<uint8_t>& state, const vector<uint8_t>& round_key) {
    for (size_t i = 0; i < 16; ++i) {
        state[i] ^= round_key[i];
    }
}

void subBytes(vector<uint8_t>& state) {
    for (size_t i = 0; i < 16; ++i) {
        state[i] = SBOX[state[i]];
    }
}

void shiftRows(vector<uint8_t>& state) {
    vector<uint8_t> temp = state;
    state[1] = temp[5];  state[5] = temp[9];  state[9] = temp[13]; state[13] = temp[1];
    state[2] = temp[10]; state[6] = temp[14]; state[10] = temp[2];  state[14] = temp[6];
    state[3] = temp[15]; state[7] = temp[3];  state[11] = temp[7];  state[15] = temp[11];
}

void mixColumns(vector<uint8_t>& state) {
    for (int i = 0; i < 4; ++i) {
        int base = i * 4;
        uint8_t s0 = state[base], s1 = state[base + 1], s2 = state[base + 2], s3 = state[base + 3];

        state[base]     = galois_mul2(s0) ^ galois_mul3(s1) ^ s2 ^ s3;
        state[base + 1] = s0 ^ galois_mul2(s1) ^ galois_mul3(s2) ^ s3;
        state[base + 2] = s0 ^ s1 ^ galois_mul2(s2) ^ galois_mul3(s3);
        state[base + 3] = galois_mul3(s0) ^ s1 ^ s2 ^ galois_mul2(s3);
    }
}

uint32_t rotWord(uint32_t word) {
    return (word << 8) | (word >> 24);
}

uint32_t subWord(uint32_t word) {
    uint32_t result = 0;
    result |= SBOX[(word >> 24) & 0xFF] << 24;
    result |= SBOX[(word >> 16) & 0xFF] << 16;
    result |= SBOX[(word >> 8) & 0xFF] << 8;
    result |= SBOX[word & 0xFF];
    return result;
}

vector<uint8_t> expandKey(const vector<uint8_t>& master_key) {
    vector<uint32_t> w(44);
    for (int i = 0; i < 4; ++i) {
        w[i] = (master_key[i*4] << 24) | (master_key[i*4+1] << 16) | (master_key[i*4+2] << 8) | master_key[i*4+3];
    }

    for (int i = 4; i < 44; ++i) {
        uint32_t temp = w[i - 1];
        if (i % 4 == 0) {
            temp = subWord(rotWord(temp)) ^ RCON[(i / 4) - 1];
        }
        w[i] = w[i - 4] ^ temp;
    }

    vector<uint8_t> expanded_keys(176);
    for (int i = 0; i < 44; ++i) {
        expanded_keys[i*4]     = (w[i] >> 24) & 0xFF;
        expanded_keys[i*4 + 1] = (w[i] >> 16) & 0xFF;
        expanded_keys[i*4 + 2] = (w[i] >> 8) & 0xFF;
        expanded_keys[i*4 + 3] = w[i] & 0xFF;
    }
    return expanded_keys;
}

vector<uint8_t> aes128_encrypt_block(const vector<uint8_t>& input_block, const vector<uint8_t>& expanded_keys) {
    vector<uint8_t> state = input_block;

    vector<uint8_t> round_key(expanded_keys.begin(), expanded_keys.begin() + 16);
    addRoundKey(state, round_key);
    printState("Start Round (After AddRoundKey)", state);

    for (int round = 1; round <= 9; ++round) {
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        
        vector<uint8_t> current_key(expanded_keys.begin() + round * 16, expanded_keys.begin() + (round + 1) * 16);
        addRoundKey(state, current_key);
        
        cout << "--- Round " << round << " complete ---" << endl;
        printState("State", state);
    }

    subBytes(state);
    shiftRows(state);
    
    vector<uint8_t> final_key(expanded_keys.begin() + 160, expanded_keys.end());
    addRoundKey(state, final_key);
    printState("Final Round (Output)", state);

    return state;
}

vector<uint8_t> applyPKCS7Padding(const string& input) {
    vector<uint8_t> data(input.begin(), input.end());
    uint8_t padding_len = 16 - (data.size() % 16);
    for (int i = 0; i < padding_len; ++i) {
        data.push_back(padding_len);
    }
    return data;
}

vector<uint8_t> processOFB(const vector<uint8_t>& data, const CryptoParams& params) {
    vector<uint8_t> result;
    result.reserve(data.size());
    vector<uint8_t> current_iv = params.iv;
    vector<uint8_t> expanded_keys = expandKey(params.key); 

    for (size_t i = 0; i < data.size(); i += 16) {
        vector<uint8_t> encrypted_iv = aes128_encrypt_block(current_iv, expanded_keys);
        
        for (size_t j = 0; j < 16 && (i + j) < data.size(); ++j) {
            result.push_back(data[i + j] ^ encrypted_iv[j]);
        }
        current_iv = encrypted_iv; 
    }
    return result;
}

bool readKeyFromFile(const string& filename, CryptoParams& params) {
    ifstream in(filename, ios::binary);
    if (!in) return false;
    params.key.resize(16);
    params.iv.resize(16);
    in.read(reinterpret_cast<char*>(params.key.data()), 16);
    in.read(reinterpret_cast<char*>(params.iv.data()), 16);
    return true;
}

string removePKCS7Padding(const vector<uint8_t>& data) {
    uint8_t padding = data.back();
    return string(data.begin(), data.end() - padding);
}

vector<uint8_t> generateRandomBytes(size_t length) {
    vector<uint8_t> bytes(length);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<uint16_t> dis(0, 255);
    for (size_t i = 0; i < length; ++i) bytes[i] = static_cast<uint8_t>(dis(gen));
    return bytes;
}

bool saveKeyToFile(const string& filename, const CryptoParams& params) {
    ofstream out(filename, ios::binary);
    if (!out) return false;
    out.write(reinterpret_cast<const char*>(params.key.data()), params.key.size());
    out.write(reinterpret_cast<const char*>(params.iv.data()), params.iv.size());
    return true;
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    
    int mode = 0;
    cout << "=== Система блочного шифрования AES-128 (OFB) ===\n";
    cout << "Выберите режим работы:\n1 - Зашифровать строку\n2 - Расшифровать строку\n3 - Выход\nВвод: ";
    cin >> mode;
    cin.ignore();

    if (mode == 1) {
        string text;
        cout << "Введите строку для шифрования: ";
        getline(cin, text);
        
        CryptoParams params;
        params.key = generateRandomBytes(16);
        params.iv = generateRandomBytes(16);

        cout << "--- Вектор инициализации (IV) ---\n";
        for (uint8_t b : params.iv) cout << hex << setw(2) << setfill('0') << (int)b << " ";
        cout << dec << "\n\n";

        vector<uint8_t> expanded_keys = expandKey(params.key);
        cout << "--- Сгенерированные раундовые ключи ---\n";
        for (int i = 0; i < 11; ++i) {
            cout << "Key " << setw(2) << i << ": ";
            for (int j = 0; j < 16; ++j) {
                cout << hex << setw(2) << setfill('0') << (int)expanded_keys[i * 16 + j] << " ";
            }
            cout << dec << "\n";
        }
        cout << "\n";
        
        vector<uint8_t> padded_data = applyPKCS7Padding(text);
        vector<uint8_t> ciphertext = processOFB(padded_data, params);

        if (saveKeyToFile("aes_key.bin", params)) {
            cout << "[УСПЕХ] Мастер-ключ и IV сохранены в файл 'aes_key.bin'\n";
        }

        cout << "Зашифрованное сообщение (HEX): ";
        for (int byte : ciphertext) {
            cout << hex << setw(2) << setfill('0') << (int)byte << " ";
        }
        cout << dec << "\n";

    } else if (mode == 2) {
        CryptoParams params;
        if (!readKeyFromFile("aes_key.bin", params)) {
            cout << "[ОШИБКА] Файл 'aes_key.bin' не найден!\n";
            return 1;
        }

        string hex_line;
        cout << "Введите зашифрованные байты в формате HEX через пробел: ";
        getline(cin, hex_line);

        stringstream ss(hex_line);
        vector<uint8_t> ciphertext;
        int hex_val;

        while (ss >> hex >> hex_val) {
            ciphertext.push_back(static_cast<uint8_t>(hex_val));
        }

        vector<uint8_t> decrypted_padded = processOFB(ciphertext, params);
        string original_text = removePKCS7Padding(decrypted_padded);

        cout << "\nРасшифрованный текст: " << original_text << endl;
    }
    
    return 0;
}