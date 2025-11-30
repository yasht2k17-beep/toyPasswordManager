#ifndef VAULT_HPP
#define VAULT_HPP

#include <string>
#include <map>
#include <vector>
#include <tuple>
#include "cbc.hpp"

class vault
{
    std::map<std::string, std::pair<std::string,std::string>> entries;
    std::vector<uint8_t> key;
    std::vector<uint8_t> iv;
    std::string fileName;

public:
    vault(const std::vector<uint8_t>& k,
          const std::vector<uint8_t>& v,
          const std::string& f);

    bool add(const std::string& site,const std::string& username,const std::string& password);

    bool save();
    bool load();

    void printAll();

    bool exists(const std::string &site) const;
    std::vector<std::tuple<std::string,std::string,std::string>> getAll() const;
};

#endif
