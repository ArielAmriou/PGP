/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** RSA
*/

#include "RSA.hpp"
#include "MyPgp.hpp"
#include "Exception.hpp"

#include <cmath>
#include <iostream>

namespace MyPgp {

    const std::pair<mpz_class, mpz_class> RSA::parseKey(std::string const &k)
    {
        auto args = MyPgp::splitString(k, '-');

        if (args.size() != 2)
            throw KeyParsingError();
        return {MyPgp::fromLittleEndianHex(args[0]),
            MyPgp::fromLittleEndianHex(args[1])};
    }

    const mpz_class RSA::genFermat(std::size_t const k)
    {
        return static_cast<mpz_class>(std::pow(2, std::pow(2, k)) + 1);
    }

    const mpz_class RSA::genE(mpz_class const &l)
    {
        for (int k = MAX_FERMAT_K; k >= 0; --k) {
            auto e = genFermat(k);

            if (e < l && gcd(e, l) == 1)
                return e;
        }
        throw GenErrorE();
    }

    const mpz_class RSA::lambda(mpz_class const &p, mpz_class const &q)
    {
        return lcm(p - 1, q - 1);
    }

    void RSA::keyGen(const std::string &p, const std::string &q)
    {
        std::pair<mpz_class, mpz_class> primes = {MyPgp::fromLittleEndianHex(p), MyPgp::fromLittleEndianHex(q)};

        std::string n = MyPgp::toLittleEndianHex(primes.first * primes.second);
        mpz_class l = lambda(primes.first, primes.second);
        mpz_class e = genE(l);
        std::string eHex = MyPgp::toLittleEndianHex(e);

        std::cout << "public key: " << MyPgp::toLittleEndianHex(e) << "-" << n << std::endl;
        std::cout << "private key: " << MyPgp::toLittleEndianHex(MyPgp::modInverse(e, l)) << "-" << n << std::endl;
    }

    const std::string RSA::encrypt(const std::string &msg, const std::string &key, [[maybe_unused]] const bool block)
    {
        auto k = parseKey(key);
        auto m = MyPgp::fromLittleEndianHex(MyPgp::strToHex(std::string(msg.rbegin(), msg.rend())));

        return MyPgp::toLittleEndianHex(MyPgp::powm(m, k.first, k.second));
    }

    const std::string RSA::decrypt(const std::string &cript, const std::string &key, [[maybe_unused]] const bool block)
    {
        auto k = parseKey(key);
        auto m = MyPgp::fromLittleEndianHex(cript);

        auto msg = MyPgp::hexToStr(MyPgp::toLittleEndianHex(MyPgp::powm(m, k.first, k.second)));
        return std::string(msg.rbegin(), msg.rend());
    }

    void RSA::checkKey(const std::string &key, [[maybe_unused]] const Mode mode)
    {
        auto k = MyPgp::splitString(key, '-');

        if (k.size() == 2 && k[0].length() >= 2 && MyPgp::isHex(k[0])
            && k[1].length() >= 2 && MyPgp::isHex(k[1])
            && MyPgp::fromLittleEndianHex(k[1]) != 0)
            return;
        throw MyPgpException("Invalid key format");
    }

}
