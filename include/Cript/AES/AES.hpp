/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** AES
*/

#ifndef AES_HPP_
#define AES_HPP_

#include "ACipher.hpp"

namespace MyPgp {

    class AES: public ACipher {
    public:
        static const std::string encrypt(const std::string &msg, const std::string &key, const bool block = false) noexcept;
        
        static const std::string decrypt(const std::string &cript, const std::string &key, const bool block = false) noexcept;
    };

};

#endif /* !XOR_HPP_ */
