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
#include <algorithm>
#include <cstdint>
#include <cstring>

#include "MyPgp.hpp"

namespace MyPgp {

    void Xor::xorBytes(std::string &dst, const std::string &src, std::size_t offset, const std::string &key, std::size_t len)
    {
        std::size_t i = 0;

        for (; i + sizeof(std::uint64_t) <= len; i += sizeof(std::uint64_t)) {
            std::uint64_t wa;
            std::uint64_t wb;

            std::memcpy(&wa, src.data() + offset + i, sizeof(wa));
            std::memcpy(&wb, key.data() + i, sizeof(wb));
            wa ^= wb;
            std::memcpy(dst.data() + offset + i, &wa, sizeof(wa));
        }
        for (; i < len; ++i)
            dst[offset + i] = src[offset + i] ^ key[i];
    }

    void Xor::applyXor(std::string &dst, const std::string &src, const std::string &key, std::size_t len)
    {
        std::size_t keylen = key.length();

        for (std::size_t offset = 0; offset < len; offset += keylen) {
            std::size_t blockLen = std::min(keylen, len - offset);

            Xor::xorBytes(dst, src, offset, key, blockLen);
        }
    }

    const std::string Xor::encrypt(const std::string &msg, const std::string &key, [[maybe_unused]] const bool block)
    {
        std::string cpy = MyPgp::hexToStr(key);
        std::size_t msglen = msg.length();
        std::size_t keylen = cpy.length();
        std::size_t paddedLen = ((msglen + keylen - 1) / keylen) * keylen;
        std::string cript(paddedLen, '\0');

        Xor::applyXor(cript, msg, cpy, msglen);
        return MyPgp::strToHex(cript);
    }

    const std::string Xor::decrypt(const std::string &cript, const std::string &key, [[maybe_unused]] const bool block)
    {
        std::string keyCpy = MyPgp::hexToStr(key);
        std::string criptCpy = MyPgp::hexToStr(cript);
        std::size_t criptlen = criptCpy.length();

        while (criptlen > 0 && criptCpy[criptlen - 1] == '\0')
            --criptlen;

        std::string msg(criptlen, '\0');
        Xor::applyXor(msg, criptCpy, keyCpy, criptlen);
        return msg;
    }

    void Xor::checkKey(const std::string &key, [[maybe_unused]] const Mode mode)
    {
        if (key.length() >= 2 && MyPgp::isHex(key))
            return;
        throw MyPgpException("Invalid key format");
    }

};
