#include "encrypt.h"
#include "decipher.h"
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cstring>
#include <iomanip>

using namespace std;

// Генерация случайного ключа
void generateRandomKey(uint8_t key[16]) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    
    for (int i = 0; i < 16; i++) {
        key[i] = static_cast<uint8_t>(dis(gen));
    }
}


void saveKeyToFile(const uint8_t key[16], const string& filename) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Не удалось открыть файл: " << filename << endl;
        return;
    }
    file.write(reinterpret_cast<const char*>(key), 16);
    file.close();
    cout << "Ключ сохранен в: " << filename << endl;
}

int main() {
    cout << "\nВыберите язык:" << endl;
    cout << "1. Русский" << endl;
    cout << "2. English" << endl;
    int langChoice;
    cin >> langChoice;
    cin.ignore();
    
    // Ввод текста
    cout << "\nВведите текст: ";
    string plaintextStr;
    getline(cin, plaintextStr);
    
    // Генерация ключа
    uint8_t key[16];
    generateRandomKey(key);
    cout << "\nСгенерированный случайный ключ" << endl;
    printBytes(key, 16, "Master Key: ");
    saveKeyToFile(key, "aes_key.bin");
    
    // Генерация IV
    uint8_t iv[16];
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    for (int i = 0; i < 16; i++) {
        iv[i] = static_cast<uint8_t>(dis(gen));
    }
    cout << "\nСгенерированный IV" << endl;
    printBytes(iv, 16, "IV: ");
    
    vector<uint8_t> plaintextBytes(plaintextStr.begin(), plaintextStr.end());
    cout << "\nИсходный текст (hex): ";
    for (uint8_t b : plaintextBytes) {
        cout << hex << setw(2) << setfill('0') << (int)b;
    }
    cout << dec << endl;
    
    
    vector<vector<uint8_t>> encryptStates;
    vector<uint8_t> ciphertext = encryptCBC(plaintextBytes, key, iv, encryptStates);
    
    cout << "\nШифротекст" << endl;
    cout << "Ciphertext (hex): ";
    for (uint8_t b : ciphertext) {
        cout << hex << setw(2) << setfill('0') << (int)b;
    }
    cout << dec << endl;
    
    // Сохранение
    ofstream cipherFile("ciphertext.bin", ios::binary);
    cipherFile.write(reinterpret_cast<const char*>(ciphertext.data()), ciphertext.size());
    cipherFile.close();
    cout << "\nШифротекст сохранен в: ciphertext.bin" << endl;
    
    vector<vector<uint8_t>> decryptStates;
    vector<uint8_t> decryptedBytes = decryptCBC(ciphertext, key, iv, decryptStates);
    string decryptedStr(decryptedBytes.begin(), decryptedBytes.end());
    
    cout << "\n Расшифрованный текст: " << decryptedStr << endl;

    cout << "ПРОМЕЖУТОЧНЫЕ СОСТОЯНИЯ" << endl;

    cout << "\nШифрование" << endl;
    for (size_t i = 0; i < encryptStates.size(); i++) {
        cout << "Block " << i << " final state: ";
        printBytes(encryptStates[i].data(), 16, "");
    }
    
    cout << "\nРасшифрование" << endl;
    for (size_t i = 0; i < decryptStates.size(); i++) {
        cout << "Block " << i << " decrypted state: ";
        printBytes(decryptStates[i].data(), 16, "");
    }
    return 0;
}