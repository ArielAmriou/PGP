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

        static const std::string encrypt(const std::string &msg, const std::string key, const bool block = false) { return ""; };
        
        static const std::string decrypt(const std::string &cript, const std::string key, const bool block = false) { return ""; };
        
    };

};

#endif /* !ACIPHER_HPP_ */
