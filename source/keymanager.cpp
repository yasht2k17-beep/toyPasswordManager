#include"../header/keymanager.hpp"
#include<iostream>
#include<iomanip>
#include<random>
#include<sstream>
#include<iomanip>

keymanager::keymanager():key(16,0),iv(16,0){}

std::vector<uint8_t>keymanager::hash(const std::string&in)const
{
    std::vector<uint8_t>h(16,0);
    uint8_t a=0;

    for(size_t i=0;i<in.size();i++)
    {
        a=static_cast<uint8_t>((a+in[i]+(i*31))&0xFF);
        h[i%16]^=a^(in[i]*13+i*7);
    }
    return h;
}

void keymanager::derive(const std::string&pw)
{
    key=hash(pw);
    iv=hash(pw+"_iv");
}
void keymanager::genRandomIV()
{
    std::random_device r;
    std::mt19937 gen(r());
    std::uniform_int_distribution<int>dist(0,255);
    for(auto&i:iv)
    {
        i=static_cast<uint8_t>(dist(gen));
    }
}

const std::vector<uint8_t>&keymanager::getKey()const
{
    return key;
}
const std::vector<uint8_t>&keymanager::getIV()const
{
    return iv;
}

void keymanager::printKey()const
{
    auto toHex=[](const std::vector<uint8_t>&data)
    {
        std::ostringstream o;
        for(auto d:data)
        {
            o<<std::hex<<std::setw(2)<<std::setfill('0')<<(int)d;
        }
        return o.str();
    };
    std::cout<<"KEY: "<<toHex<<"\n";
    std::cout<<"IV: "<<toHex<<"\n";
}