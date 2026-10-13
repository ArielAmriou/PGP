/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** Rsa
*/

#ifndef RSA_HPP_
#define RSA_HPP_

#include "ACipher.hpp"
#include <gmpxx.h>

namespace MyPgp {

    constexpr std::size_t MAX_FERMAT_K = 4;

    class RSA: public ACipher {
    private:

        static const mpz_class genFermat(std::size_t const k);

        static const mpz_class genE(mpz_class const &l);

        static const std::pair<mpz_class, mpz_class> parseKey(std::string const &k);

    public:

        static const mpz_class lambda(mpz_class const &p, mpz_class const &q);

        static const std::string encrypt(const std::string &msg, const std::string &key, const bool block = false);
        
        static const std::string decrypt(const std::string &cript, const std::string &key, const bool block = false);

        static void keyGen(const std::string &p, const std::string &q);

        static void checkKey(const std::string &key, [[maybe_unused]] const Mode mode);

    };

};

#endif /* !RSA_HPP_ */
