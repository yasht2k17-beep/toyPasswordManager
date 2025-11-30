#ifndef CBC_H
#define CBC_H

#include <vector>
#include <cstdint>
#include "AES.hpp"

class CBC {
private:
    std::vector<uint8_t> key;
    std::vector<uint8_t> iv;

    std::vector<std::vector<uint8_t>> makeBlocks(const std::vector<uint8_t>& in) const;

public:
    CBC(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv);
    CBC();

    void setKey(const std::vector<uint8_t>& newKey);
    void setIV(const std::vector<uint8_t>& newIV);

    std::vector<std::vector<uint8_t>> encrypt(const std::vector<uint8_t>& plaintext);

    std::vector<uint8_t> decrypt(const std::vector<std::vector<uint8_t>>& ciphertext);
};

#endif
