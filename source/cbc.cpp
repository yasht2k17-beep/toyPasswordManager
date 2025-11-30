#include "../header/cbc.hpp"
#include <algorithm>

CBC::CBC(const std::vector<uint8_t>& key, const std::vector<uint8_t>& iv): key(key), iv(iv) 
{
    if (this->key.size() != 16)
     this->key.resize(16, 0);
    if (this->iv.size() != 16) 
    this->iv.resize(16, 0);
}

CBC::CBC() : key(16, 0), iv(16, 0) {}

void CBC::setKey(const std::vector<uint8_t>& newKey)
 {
    key = newKey;
    if (key.size() != 16)
     key.resize(16, 0);
}

void CBC::setIV(const std::vector<uint8_t>& newIV) 
{
    iv = newIV;
    if (iv.size() != 16) 
    iv.resize(16, 0);
}

std::vector<std::vector<uint8_t>> CBC::makeBlocks(const std::vector<uint8_t>& in) const 
{
    std::vector<std::vector<uint8_t>> blocks;
    int n = (in.size() + 15) / 16;
    for (int i = 0; i < n; i++) 
    {
        auto start = in.begin() + 16 * i;
        auto end = ((i + 1) * 16 <= (int)in.size() ? start + 16 : in.end());
        std::vector<uint8_t> block(start, end);
        if (block.size() < 16) 
        {
            uint8_t pad = uint8_t(16 - block.size());
            block.insert(block.end(), pad, pad);
        }
        blocks.push_back(block);
    }
    return blocks;
}

std::vector<std::vector<uint8_t>> CBC::encrypt(const std::vector<uint8_t>& plaintext) 
{
    std::vector<std::vector<uint8_t>> out;
    AES aes;
    auto blocks = makeBlocks(plaintext);
    std::vector<uint8_t> prev = iv;

    for (auto& blk : blocks) 
    {
        std::vector<uint8_t> x(16);
        for (int i = 0; i < 16; i++)
            x[i] = blk[i] ^ prev[i];

        auto c = aes.encryptBlock(x, key);
        out.push_back(c);
        prev = c;
    }
    return out;
}
std::vector<uint8_t> CBC::decrypt(const std::vector<std::vector<uint8_t>>& ciphertext) 
{
    std::vector<uint8_t> out;
    AES aes;
    std::vector<uint8_t> prev = iv;

    for (auto& blk : ciphertext) 
    {
        auto p = aes.decryptBlock(blk, key);
        std::vector<uint8_t> plain(16);
        for (int i = 0; i < 16; i++)
            plain[i] = p[i] ^ prev[i];

        out.insert(out.end(), plain.begin(), plain.end());
        prev = blk;
    }

    if (!out.empty()) 
    {
        uint8_t pad = out.back();
        if (pad >= 1 && pad <= 16 &&std::all_of(out.end() - pad, out.end(), [pad](uint8_t b) { return b == pad; })) 
        {
            out.resize(out.size() - pad);
        }
    }
    return out;
}
