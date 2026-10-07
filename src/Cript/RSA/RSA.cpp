/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** RSA
*/

#include "RSA.hpp"
#include "MyPgp.hpp"
#include "Exception.hpp"

#include <boost/integer/mod_inverse.hpp>

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
        return static_cast<cpp_int>(std::pow(2, std::pow(2, k)) + 1);
    }

    const cpp_int RSA::genE(cpp_int const &l)
    {
        for (int k = MAX_FERMAT_K; k >= 0; --k) {
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
        
        std::string n = MyPgp::toLittleEndianHex(primes.first * primes.second);
        cpp_int l = lambda(primes.first, primes.second);
        cpp_int e = genE(l);
        std::string eHex = MyPgp::toLittleEndianHex(e);

        std::cout << "public key: " << MyPgp::toLittleEndianHex(e) << "-" << n << std::endl;
        std::cout << "private key: " << MyPgp::toLittleEndianHex(boost::integer::mod_inverse(e, l)) << "-" << n << std::endl;
    }

    const std::string RSA::encrypt(const std::string &msg, const std::string &key, [[maybe_unused]] const bool block)
    {
        auto k = parseKey(key);
        auto m = MyPgp::fromLittleEndianHex(MyPgp::strToHex(std::string(msg.rbegin(), msg.rend())));

        return MyPgp::toLittleEndianHex(boost::multiprecision::powm(m, k.first, k.second));
    }
        
    const std::string RSA::decrypt(const std::string &cript, const std::string &key, [[maybe_unused]] const bool block)
    {
        auto k = parseKey(key);
        auto m = MyPgp::fromLittleEndianHex(cript);

        auto msg = MyPgp::hexToStr(MyPgp::toLittleEndianHex(boost::multiprecision::powm(m, k.first, k.second)));
        return std::string(msg.rbegin(), msg.rend());
    }

    void RSA::checkKey(const std::string &key, [[maybe_unused]] const Mode mode)
    {
        auto k = MyPgp::splitString(key, '-');

        if (k.size() == 2 && k[0].length() >= 2 && MyPgp::isHex(k[0])
            && k[1].length() >= 2 && MyPgp::isHex(k[1]))
            return;
        throw MyPgpException("Invalid key format");
    }

}
