/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** Cipĥer
*/

#ifndef ACIPHER_HPP_
#define ACIPHER_HPP_

#include <string>

namespace MyPgp {

    class ACipher {
    public:

        static const std::string encrypt(const std::string key, const bool block = false) noexcept {};
        
        static const std::string decrypt(const std::string key, const bool block = false) noexcept {};
        
    };

};

#endif /* !ACIPHER_HPP_ */
