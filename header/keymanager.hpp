#ifndef KEYMANAGER_H
#define KEYMANAGER_H
#include<string>
#include<vector>
#include<cstdint>

class keymanager
{
    private:
    std::vector<uint8_t>key;
    std::vector<uint8_t>iv;

    std::vector<uint8_t>hash(const std::string&in)const;

    public:
    keymanager();

    void derive(const std::string&pw);
    void genRandomIV();

    const std::vector<uint8_t>&getKey()const;
    const std::vector<uint8_t>&getIV()const;

    void printKey()const;
};

#endif