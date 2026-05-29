#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <vector>
#include <string>
#include <cstdint>


const int BLOCK_SIZE = 16;
extern const uint8_t S_BOX[256];
extern const uint8_t INV_S_BOX[256];


void expandKey(const uint8_t* key, uint8_t roundKeys[11][16]);
void encryptBlock(const uint8_t* input, uint8_t* output, const uint8_t roundKeys[11][16]);

std::vector<uint8_t> encryptCBC(const std::vector<uint8_t>& plaintext, const uint8_t key[16], const uint8_t iv[16], std::vector<std::vector<uint8_t>>& intermediateStates);
std::vector<uint8_t> pad(const std::vector<uint8_t>& data);


void printBytes(const uint8_t* data, int len, const std::string& name);

#endif