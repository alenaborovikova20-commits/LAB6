#include "decipher.h"
#include "encrypt.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>

using namespace std;


static void invSubBytes(uint8_t* state) {
    for (int i = 0; i < 16; i++) {
        state[i] = INV_S_BOX[state[i]];
    }
}

static void invShiftRows(uint8_t* state) {
    uint8_t tmp;
    
    tmp = state[13];
    state[13] = state[9];
    state[9] = state[5];
    state[5] = state[1];
    state[1] = tmp;
    
    tmp = state[2];
    state[2] = state[10];
    state[10] = tmp;
    tmp = state[6];
    state[6] = state[14];
    state[14] = tmp;
    
    tmp = state[7];
    state[7] = state[11];
    state[11] = state[15];
    state[15] = state[3];
    state[3] = tmp;
}

static uint8_t gfMultDec(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) p ^= a;
        bool hi = a & 0x80;
        a <<= 1;
        if (hi) a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

static void invMixColumns(uint8_t* state) {
    for (int c = 0; c < 4; c++) {
        int idx = c * 4;
        uint8_t s0 = state[idx];
        uint8_t s1 = state[idx+1];
        uint8_t s2 = state[idx+2];
        uint8_t s3 = state[idx+3];
        
        state[idx]   = gfMultDec(0x0e, s0) ^ gfMultDec(0x0b, s1) ^ gfMultDec(0x0d, s2) ^ gfMultDec(0x09, s3);
        state[idx+1] = gfMultDec(0x09, s0) ^ gfMultDec(0x0e, s1) ^ gfMultDec(0x0b, s2) ^ gfMultDec(0x0d, s3);
        state[idx+2] = gfMultDec(0x0d, s0) ^ gfMultDec(0x09, s1) ^ gfMultDec(0x0e, s2) ^ gfMultDec(0x0b, s3);
        state[idx+3] = gfMultDec(0x0b, s0) ^ gfMultDec(0x0d, s1) ^ gfMultDec(0x09, s2) ^ gfMultDec(0x0e, s3);
    }
}

static void addRoundKeyDec(uint8_t* state, const uint8_t* key) {
    for (int i = 0; i < 16; i++) {
        state[i] ^= key[i];
    }
}


void decryptBlock(const uint8_t* input, uint8_t* output, const uint8_t roundKeys[11][16]) {
    uint8_t state[16];
    memcpy(state, input, 16);
    
    cout << "\nDECRYPT BLOCK" << endl;
    printBytes(state, 16, "Initial State: ");
    
    addRoundKeyDec(state, roundKeys[10]);
    printBytes(state, 16, "After AddRoundKey[10]: ");
    
    for (int round = 9; round >= 1; round--) {
        cout << "\nRound " << round << endl;
        invShiftRows(state);
        printBytes(state, 16, "After InvShiftRows: ");
        invSubBytes(state);
        printBytes(state, 16, "After InvSubBytes:  ");
        addRoundKeyDec(state, roundKeys[round]);
        printBytes(state, 16, "After AddRoundKey:  ");
        invMixColumns(state);
        printBytes(state, 16, "After InvMixColumns: ");
    }
    
    cout << "\nRound 0 (Final)" << endl;
    invShiftRows(state);
    printBytes(state, 16, "After InvShiftRows: ");
    invSubBytes(state);
    printBytes(state, 16, "After InvSubBytes:  ");
    addRoundKeyDec(state, roundKeys[0]);
    printBytes(state, 16, "After AddRoundKey[0]: ");
    
    memcpy(output, state, 16);
}

vector<uint8_t> unpad(const vector<uint8_t>& data) {
    if (data.empty()) return data;
    
    uint8_t padding = data.back();
    if (padding > BLOCK_SIZE || padding == 0) {
        return data;  
    }
    
    return vector<uint8_t>(data.begin(), data.end() - padding);
}


vector<uint8_t> decryptCBC(const vector<uint8_t>& ciphertext,
                            const uint8_t key[16],
                            const uint8_t iv[16],
                            vector<vector<uint8_t>>& intermediateStates) {
    uint8_t roundKeys[11][16];
    expandKey(key, roundKeys);
    
    vector<uint8_t> plaintext;
    uint8_t previous[16];
    memcpy(previous, iv, 16);
    
    for (size_t i = 0; i < ciphertext.size(); i += BLOCK_SIZE) {
        uint8_t block[16];
        memcpy(block, &ciphertext[i], 16);
        
        cout << "\nCBC Decrypt Block " << (i / BLOCK_SIZE) + 1  << endl;
        printBytes(block, 16, "Ciphertext block: ");
        
        uint8_t decrypted[16];
        decryptBlock(block, decrypted, roundKeys);
        
        vector<uint8_t> stateVec(decrypted, decrypted + 16);
        intermediateStates.push_back(stateVec);
        printBytes(decrypted, 16, "After decryption: ");
        
        for (int j = 0; j < 16; j++) {
            decrypted[j] ^= previous[j];
        }
        printBytes(decrypted, 16, "After XOR with previous: ");
        
        plaintext.insert(plaintext.end(), decrypted, decrypted + 16);
        memcpy(previous, block, 16);
    }
    
    return unpad(plaintext);
}


void loadKeyFromFile(const string& filename, uint8_t key[16]) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Не удалось открыть файл: " << filename << endl;
        return;
    }
    file.read(reinterpret_cast<char*>(key), 16);
    file.close();
    cout << "Ключ загружен из: " << filename << endl;
    printBytes(key, 16, "Loaded key: ");
}