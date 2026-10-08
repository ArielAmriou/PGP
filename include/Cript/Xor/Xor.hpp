/*
** EPITECH PROJECT, 2026
** $
** File description:
** Xor
*/

#ifndef XOR_HPP_
#define XOR_HPP_

#include "ACipher.hpp"

namespace MyPgp {

    class Xor: public ACipher {
    public:

        static const std::string encrypt(const std::string &msg, const std::string &key, [[maybe_unused]] const bool block = false);
        
        static const std::string decrypt(const std::string &cript, const std::string &key, [[maybe_unused]] const bool block = false);

        static void checkKey(const std::string &key, [[maybe_unused]] const Mode mode);

    private:

        static void xorBytes(std::string &dst, const std::string &src, std::size_t offset, const std::string &key, std::size_t len);

        static void applyXor(std::string &dst, const std::string &src, const std::string &key, std::size_t len);
    };

};

#endif /* !XOR_HPP_ */
