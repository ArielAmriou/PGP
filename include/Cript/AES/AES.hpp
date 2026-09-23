/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** AES
*/

#ifndef AES_HPP_
#define AES_HPP_

#include <array>
#include <vector>
#include "ACipher.hpp"
#include "Matrix.hpp"

namespace MyPgp {

    class AES: public ACipher {
    public:
        static const std::string encrypt(const std::string &msg, const std::string &key, const bool block = false) noexcept;
        
        static const std::string decrypt(const std::string &cript, const std::string &key, const bool block = false) noexcept;
    private:
        using Block = Matrix<4, 4>;

        std::string &xorWord(std::string &word, const std::string &key);
        std::string &subWord(std::string &word);
        std::string &rotWord(std::string &word);
        std::string &rcon(std::string &word, const std::size_t round);
        void keyExpansion(const std::string &key);
        Block &shiftRows(Block &block);
        
        std::vector<std::string> _keys;
        static constexpr std::size_t NBROUND = 6;
        static constexpr std::size_t WORDSIZE = 4;
        static constexpr std::size_t SBOXSIZE = 256;
        static const std::array<u_int8_t, SBOXSIZE> SBOX;
        static constexpr std::size_t RCONSIZE = 10;
        static const std::array<u_int8_t, RCONSIZE> RCON;
    };
};

#endif /* !XOR_HPP_ */
