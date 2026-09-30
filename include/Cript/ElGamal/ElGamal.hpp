/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** ElGamal
*/

#ifndef ELGAMAL_HPP_
#define ELGAMAL_HPP_

#include <boost/multiprecision/cpp_int.hpp>
#include <optional>
#include "ACipher.hpp"

namespace MyPgp {

    using boost::multiprecision::cpp_int;
    using boost::multiprecision::powm;
    using boost::multiprecision::msb;

    class ElGamal : public ACipher {
    public:

        static const std::string encrypt(const std::string &msg, const std::string &key, const bool block = false);
        
        static const std::string decrypt(const std::string &cript, const std::string &key, const bool block = false);

        static void keyGen(const std::string &p);

    private:
        enum ENCRYPT_INFO {
            ENCRYPT_PRIME,
            ENCRYPT_GENERATOR,
            ENCRYPT_PUBLIC,
            NB_ENCRYPT_INFO
        };

        enum DECRYPT_MSG_INFO {
            DECRYPT_C1,
            DECRYPT_C2,
            NB_DECRYPT_MSG_INFO
        };

        enum DECRYPT_KEY_INFO {
            DECRYPT_PRIME,
            DECRYPT_GENERATOR,
            DECRYPT_PRIVATE,
            NB_DECRYPT_KEY_INFO
        };

        static cpp_int findGenerator(const cpp_int &p);
        static cpp_int getRange(const cpp_int &min, const cpp_int &max,
            const std::optional<unsigned int> &seed);
        static std::vector<cpp_int> parseInfo(
            const std::vector<std::string> &infos, std::size_t nb);

        static constexpr std::size_t BYTESIZE = 8;
    };
}

#endif /* !ELGAMAL_HPP_ */
