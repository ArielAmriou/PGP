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

#include "MyPgp.hpp"

namespace MyPgp {

    const std::string Xor::encrypt(const std::string &msg, const std::string &key, [[maybe_unused]] const bool block)
    {
        std::string cpy = MyPgp::hexToStr(key);
        std::size_t i = 0;
        std::size_t msglen = msg.length();
        std::size_t keylen = cpy.length();
        std::string cript;

        for (; i < msglen; ++i) {
            char xorByte = msg[i] ^ cpy[i % keylen];
            cript.push_back(xorByte);
        }
        for (; i % keylen != 0; ++i)
            cript.push_back('\0');
        return MyPgp::strToHex(cript);
    }

    const std::string Xor::decrypt(const std::string &cript, const std::string &key, [[maybe_unused]] const bool block)
    {
        std::string keyCpy = MyPgp::hexToStr(key);
        std::string criptCpy = MyPgp::hexToStr(cript);
        std::size_t criptlen = criptCpy.length();
        std::size_t keylen = keyCpy.length();
        std::string msg;

        while (criptlen > 0 && criptCpy[criptlen - 1] == '\0')
            --criptlen;
        for (std::size_t i = 0; i < criptlen; ++i) {
            unsigned int byte;
            byte = criptCpy[i];
            msg.push_back(static_cast<char>(byte) ^ keyCpy[i % keylen]);
        }
        return msg;
    }

    void Xor::checkKey(const std::string &key, [[maybe_unused]] const Mode mode)
    {
        if (key.length() >= 2 && MyPgp::isHex(key))
            return;
        throw MyPgpException("Invalid key format");
    }

};
