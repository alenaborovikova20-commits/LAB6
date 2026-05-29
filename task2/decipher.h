#ifndef DECIPHER_H
#define DECIPHER_H

#include <vector>
#include <string>
#include <cstdint>


void decryptBlock(const uint8_t* input, uint8_t* output, const uint8_t roundKeys[11][16]);


std::vector<uint8_t> decryptCBC(const std::vector<uint8_t>& ciphertext,
                                 const uint8_t key[16],
                                 const uint8_t iv[16],
                                 std::vector<std::vector<uint8_t>>& intermediateStates);


std::vector<uint8_t> unpad(const std::vector<uint8_t>& data);


void loadKeyFromFile(const std::string& filename, uint8_t key[16]);

#endif