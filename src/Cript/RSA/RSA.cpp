/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** RSA
*/

#include "RSA.hpp"
#include "MyPgp.hpp"
#include "Exception.hpp"

namespace MyPgp {

    const std::pair<cpp_int, cpp_int> RSA::parseKey(std::string const &k)
    {
        auto args = MyPgp::splitString(k, '-');

        if (args.size() != 2)
            throw KeyParsingError();
        return {MyPgp::fromLittleEndianHex(args[0]),
            MyPgp::fromLittleEndianHex(args[1])};
    }

    const cpp_int RSA::genFermat(std::size_t const k)
    {
        return static_cast<cpp_int>(std::pow(2, std::pow(2, k) + 1));
    }

    const cpp_int RSA::genE(cpp_int const &l)
    {
        for (std::size_t k = MAX_FERMAT_K; k >= 0; --k) {
            auto e = genFermat(k);

            if (e < l && boost::multiprecision::gcd(e, l) == 1)
                return e;
        }
        throw GenErrorE();
    }

    const cpp_int RSA::lambda(cpp_int const &p, cpp_int const &q)
    {
        return boost::multiprecision::lcm(p - 1, q - 1);
    }

    void RSA::keyGen(const std::string &p, const std::string &q)
    {
        std::pair<cpp_int, cpp_int> primes = {MyPgp::fromLittleEndianHex(p), MyPgp::fromLittleEndianHex(q)};
        cpp_int n = primes.first * primes.second;

        cpp_int l = lambda(primes.first, primes.second);
        cpp_int e = genE(l);


    }

    const std::string RSA::encrypt(const std::string &msg, const std::string &key, const bool block)
    {
        auto k = parseKey(key);
        auto m = MyPgp::fromLittleEndianHex(MyPgp::strToHex(std::string(msg.rbegin(), msg.rend())));

        return MyPgp::toLittleEndianHex(boost::multiprecision::powm(m, k.first, k.second));
    }
        
    const std::string RSA::decrypt(const std::string &cript, const std::string &key, const bool block)
    {
        auto k = parseKey(key);
        auto m = MyPgp::fromLittleEndianHex(cript);

        auto msg = MyPgp::hexToStr(MyPgp::toLittleEndianHex(boost::multiprecision::powm(m, k.first, k.second)));
        return std::string(msg.rbegin(), msg.rend());
    }

}
