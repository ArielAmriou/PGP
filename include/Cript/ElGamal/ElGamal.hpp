/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** ElGamal
*/

#ifndef ELGAMAL_HPP_
#define ELGAMAL_HPP_

#include <boost/multiprecision/cpp_int.hpp>
#include "ACipher.hpp"

namespace MyPgp {

    using boost::multiprecision::cpp_int;
    using boost::multiprecision::powm;

    class ElGamal : public ACipher {
    public:

        static const std::string encrypt(const std::string &msg, const std::string &key, const bool block = false);
        
        static const std::string decrypt(const std::string &cript, const std::string &key, const bool block = false);

        static void keyGen(const std::string &p);

    private:
        static cpp_int findGenerator(const cpp_int &p);

    };
}

#endif /* !ELGAMAL_HPP_ */
