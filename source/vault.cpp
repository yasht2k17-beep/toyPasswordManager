#include "../header/vault.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

vault::vault(const std::vector<uint8_t>& k,const std::vector<uint8_t>& v,const std::string& f)
{
    key = k;
    iv  = v;
    fileName = f;
}

bool vault::exists(const std::string &site) const 
{
    return entries.find(site) != entries.end();
}

bool vault::add(const std::string& site,const std::string& username,const std::string& password)
{
    entries[site] = {username, password};
    return true;
}

void vault::printAll()
{
    for (auto &i : entries)
    {
        std::cout << "Site: "     << i.first
                  << "\nUser: "   << i.second.first
                  << "\nPass: "   << i.second.second
                  << "\n----------------------\n";
    }
}

bool vault::save()
{
    std::ostringstream o;

    for (auto &i : entries)
    {
        o << i.first << "\n"
          << i.second.first << "\n"
          << i.second.second << "\n";
    }

    std::string plain = o.str();
    std::vector<uint8_t> pBytes(plain.begin(), plain.end());

    CBC cbc(key, iv);
    auto blocks = cbc.encrypt(pBytes);

    std::ofstream out(fileName, std::ios::binary);
    
    if (!out) 
    return false;

    uint32_t count = blocks.size();
    out.write((char*)&count, sizeof(count));

    out.write((char*)iv.data(), 16);

    for (auto &b : blocks)
        out.write((char*)b.data(), 16);

    return true;
}

bool vault::load()
{
    std::ifstream in(fileName, std::ios::binary);

    if (!in) 
    return false;

    uint32_t count = 0;
    in.read((char*)&count, sizeof(count));
    if (!in || count == 0) 
    return false;

    std::vector<uint8_t> fileIV(16);
    in.read((char*)fileIV.data(), 16);

    std::vector<std::vector<uint8_t>> blocks(count, std::vector<uint8_t>(16));

    for (uint32_t i = 0; i < count; i++)
        in.read((char*)blocks[i].data(), 16);

    CBC cbc(key, fileIV);
    auto plain = cbc.decrypt(blocks);

    std::string s(plain.begin(), plain.end());
    std::istringstream iss(s);

    entries.clear();
    std::string site, user, pass;

    while (std::getline(iss, site))
    {
        if (!std::getline(iss, user)) 
        break;
        if (!std::getline(iss, pass)) 
        break;
        entries[site] = {user, pass};
    }
    return true;
}
std::vector<std::tuple<std::string,std::string,std::string>> vault::getAll() const 
{
    std::vector<std::tuple<std::string,std::string,std::string>> out;
    out.reserve(entries.size());
    for (const auto &p : entries) 
    {
        out.emplace_back(p.first, p.second.first, p.second.second);
    }
    return out;
}