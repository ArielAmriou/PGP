/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** ElGamal
*/

#include <optional>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include "ElGamal.hpp"
#include "Exception.hpp"
#include "MyPgp.hpp"

namespace MyPgp {

    const std::string ElGamal::encrypt(const std::string &msg, const std::string &key, const bool block)
    {
        return "";
    }
        
    const std::string ElGamal::decrypt(const std::string &cript, const std::string &key, const bool block)
    {
        return "";
    }

    void ElGamal::keyGen(const std::string &p)
    {
            auto args = MyPgp::splitString(p, ':');

            if (args.empty() || args.size() > 2)
                throw MyPgpException("Wrong args.");
            cpp_int prime = MyPgp::fromLittleEndianHex(args[0]);
            std::optional<unsigned int> seed;
            if (args.size() == 2)
                seed = MyPgp::fromLittleEndianHex(args[1]).convert_to<unsigned int>();
            boost::random::mt19937 gen(seed ? seed.value() : std::time(nullptr));
            boost::random::uniform_int_distribution<cpp_int> distrib(2, prime - 2);
            try {
                cpp_int generator = findGenerator(prime);
                cpp_int privateKey = distrib(gen);
                cpp_int publicKey = powm(generator, privateKey, prime);
                std::cout << "Generator: " << MyPgp::toLittleEndianHex(generator) << "\n";
                std::cout << "Private key: " << MyPgp::toLittleEndianHex(privateKey) << "\n";
                std::cout << "Public key: " << MyPgp::toLittleEndianHex(publicKey) << "\n";
            } catch (MyPgpException &e) {
                throw e;
            }
    }

    cpp_int ElGamal::findGenerator(const cpp_int &p)
    {
        cpp_int q = (p - 1) / 2;

        for (cpp_int g = 2; g < p - 1; ++g) {
            if (powm(g, cpp_int(2), p) != 1 && powm(g, q, p) != 1)
                return g;
        }
        throw MyPgpException("No generator found");
    }
}