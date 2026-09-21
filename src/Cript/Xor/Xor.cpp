/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** Xor
*/

#include "Xor.hpp"

#include <sstream>
#include <ios>
#include <iostream>

namespace MyPgp {

    const std::string Xor::encrypt(const std::string &msg, const std::string &key, const bool block) noexcept
    {
        std::size_t i = 0;
        std::size_t msglen = msg.length();
        std::size_t keylen = key.length();
        std::string cript;

        for (; i < msglen; ++i) {
            char msgByte = msg[i];
            char keyByte = key[i % keylen];

            char xorByte = msgByte ^ keyByte;
            
            std::stringstream ss;
            ss << std::hex << static_cast<int>(xorByte);
            std::string strByte = ss.str();
            if (strByte.length() < 2)
                strByte = "0" + strByte;

            cript.append(strByte);
        }
        for (; i % keylen != 0; ++i)
            cript.push_back('\0');
        return cript;
    }

    const std::string Xor::decrypt(const std::string &cript, const std::string &key, const bool block) noexcept
    {
        std::string msg;
        std::string bytes;
        std::size_t criptlen = cript.length();
        std::size_t keylen = key.length();
        
        for (std::size_t i = 0; i < criptlen && cript[i] != '\0' ; i += 2) {
            unsigned int byte;

            std::stringstream ss;
            ss << std::hex << cript.substr(i, 2);
            ss >> byte;
            msg.push_back(static_cast<char>(byte) ^ key[(i / 2) % keylen]);
        }
        
        return msg;
    }

};
