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
            char xorByte = msg[i] ^ key[i % keylen];
            
            cript.push_back(xorByte);
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
        
        for (std::size_t i = 0; i < criptlen ; ++i) {
            unsigned int byte;

            byte = cript[i];
            msg.push_back(static_cast<char>(byte) ^ key[i % keylen]);
        }
        
        return msg;
    }

};
