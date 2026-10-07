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

    enum class Mode;
    class ACipher {
    public:
        using CipherFn = const std::string (&)(const std::string &, const std::string &, const bool);
        using KeyCheckFn = void (&)(const std::string &, const Mode);

        static const std::string encrypt([[maybe_unused]] const std::string &msg,
            [[maybe_unused]] const std::string key,
            [[maybe_unused]] const bool block = false) { return ""; };
        
        static const std::string decrypt([[maybe_unused]] const std::string &cript,
            [[maybe_unused]] const std::string key,
            [[maybe_unused]] const bool block = false) { return ""; };
        
        static void keyGen([[maybe_unused]] const std::string &p) {};
        
        static void keyGen([[maybe_unused]] const std::string &p,
            [[maybe_unused]] const std::string &q) {};
        
        static void checkKey(const std::string &key, [[maybe_unused]] const Mode mode);
    };

};

#endif /* !ACIPHER_HPP_ */
