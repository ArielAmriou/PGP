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

        static const std::string encrypt(const std::string &msg, const std::string &key, const bool block = false);
        
        static const std::string decrypt(const std::string &cript, const std::string &key, const bool block = false);
    };

};

#endif /* !XOR_HPP_ */
