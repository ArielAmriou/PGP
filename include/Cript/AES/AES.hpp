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
#include "Exception.hpp"

namespace MyPgp {

    class AES: public ACipher {
    public:
        AES() : _mixColumns(MIXCOLUMNS), _unmixColumns(UNMIXCOLUMNS) {};

        static const std::string encrypt(const std::string &msg, const std::string &key, const bool block = false);
        
        static const std::string decrypt(const std::string &cript, const std::string &key, const bool block = false);

        class WrongKeySize : public MyPgpException {
        public:
            WrongKeySize() :
                MyPgpException("The key is not of the right size. Must be 16, 24 or 32 bytes.") {};
        };
    private:
        using Block = Matrix<4, 4>;

        std::string &xorWord(std::string &word, const std::string &key);
        std::string &subWord(std::string &word);
        std::string &rotWord(std::string &word);
        std::string &rcon(std::string &word, const std::size_t round);
        void keyExpansion(const std::string &key);
        Block &shiftRows(Block &block);
        static char gMul(char a, char b);
        static char xorChar(char a, char b);
        Block &mixColumns(Block &block);
        std::string encryptBlock(const std::string &str);
        std::string &unsubWord(std::string &word);
        Block &unshiftRows(Block &block);
        Block &unmixColumns(Block &block);
        std::string decryptBlock(const std::string &str);
        
        std::vector<std::string> _keys;
        Block _mixColumns;
        Block _unmixColumns;

        static constexpr std::size_t NBROUND = 6;
        static constexpr std::size_t WORDSIZE = 4;
        static constexpr std::size_t BLOCKSIZE = 16;
        static constexpr std::size_t SBOXSIZE = 256;
        static const std::array<u_int8_t, SBOXSIZE> SBOX;
        static constexpr std::size_t RCONSIZE = 10;
        static const std::array<u_int8_t, RCONSIZE> RCON;
        static const std::array<u_int8_t, BLOCKSIZE> MIXCOLUMNS;
        static const std::array<u_int8_t, BLOCKSIZE> UNMIXCOLUMNS;
        static const std::vector<std::size_t> KEYSIZES;
    };
};

#endif /* !XOR_HPP_ */
