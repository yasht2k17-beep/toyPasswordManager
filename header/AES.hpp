#ifndef AES_H
#define AES_H

#include <vector>
#include <cstdint>

class AES {
private:
    std::vector<uint8_t> state;
    static const uint8_t sbox[256];
    static const uint8_t inv_sbox[256];
    static const uint8_t Rcon[11];

    void subByte();
    void shiftRows();
    uint8_t xtime(uint8_t x);
    void mixColumns();
    void addRoundKey(const std::vector<uint8_t>& roundKey);

    std::vector<std::vector<uint8_t>> keyExpansion(const std::vector<uint8_t>& key);

    void invSubByte();
    void invShiftRows();
    uint8_t mul(uint8_t a, uint8_t b);
    void invMixColumns();

public:
    AES();
    AES(const std::vector<uint8_t>& inp);

    void setState(const std::vector<uint8_t>& inp);
    std::vector<uint8_t> getState() const;

    std::vector<uint8_t> encryptBlock(const std::vector<uint8_t>& block, const std::vector<uint8_t>& key);
    std::vector<uint8_t> decryptBlock(const std::vector<uint8_t>& block, const std::vector<uint8_t>& key);

    void encrypt(const std::vector<uint8_t>& key);
    void decrypt(const std::vector<uint8_t>& key);
};

#endif
