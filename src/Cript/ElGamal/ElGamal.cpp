/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** ElGamal
*/

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <random>
#include "ElGamal.hpp"
#include "Exception.hpp"
#include "MyPgp.hpp"

namespace MyPgp {

    const std::string ElGamal::encrypt(const std::string &msg, const std::string &key, const bool block)
    {
        auto args = MyPgp::splitString(key, ':');

        if (args.empty() || args.size() > 2)
            throw MyPgpException("Wrong args.");
        auto infos = parseInfo(MyPgp::splitString(args[0], '-'), ENCRYPT_INFO::NB_ENCRYPT_INFO);
        std::optional<unsigned int> seed;
        if (args.size() == 2)
            seed = MyPgp::fromLittleEndianHex(args[1]).convert_to<unsigned int>();
        cpp_int tmpKey = getRange(2, infos[ENCRYPT_PRIME], seed);
        cpp_int c1 = powm(infos[ENCRYPT_GENERATOR], tmpKey, infos[ENCRYPT_PRIME]);
        std::string crypt = MyPgp::toLittleEndianHex(c1) + "\n";
        cpp_int secret = powm(infos[ENCRYPT_PUBLIC], tmpKey, infos[ENCRYPT_PRIME]);
        std::size_t byteLen = msb(infos[ENCRYPT_PRIME] - 1) / BYTESIZE + 1;
        for (unsigned char ch : msg) {
            cpp_int m = ch;
            if (m >= infos[ENCRYPT_PRIME])
                throw MyPgpException("Prime too small for this message.");
            cpp_int c2 = (m * secret) % infos[ENCRYPT_PRIME];
            crypt += MyPgp::toLittleEndianHex(c2, byteLen);
        }
        return crypt;
    }
        
    const std::string ElGamal::decrypt(const std::string &cript, const std::string &key, const bool block)
    {
        auto msgInfos = MyPgp::splitString(cript, '-');
        auto keyInfos = parseInfo(MyPgp::splitString(key, '-'), NB_DECRYPT_KEY_INFO);
        if (msgInfos.size() != NB_DECRYPT_MSG_INFO)
            throw MyPgpException("Wrong infos.");
        cpp_int c1 = MyPgp::fromLittleEndianHex(msgInfos[DECRYPT_C1]);
        cpp_int secret = powm(c1, keyInfos[DECRYPT_PRIVATE], keyInfos[DECRYPT_PRIME]);
        cpp_int invSecret = powm(secret, keyInfos[DECRYPT_PRIME] - 2, keyInfos[DECRYPT_PRIME]);
        std::size_t hexLen = (msb(keyInfos[DECRYPT_PRIME] - 1) / BYTESIZE + 1) * 2;
        std::string decrypt;
        std::string body(msgInfos[DECRYPT_C2]);
        if (body.size() % hexLen != 0)
            throw MyPgpException("Wrong message length.");
        for (std::size_t i = 0; i < body.size(); i += hexLen) {
            cpp_int c2 = MyPgp::fromLittleEndianHex(body.substr(i, hexLen));
            cpp_int m = (c2 * invSecret) % keyInfos[DECRYPT_PRIME];
            if (m > 255)
                throw MyPgpException("Decryption failed.");
            decrypt += static_cast<char>(m.convert_to<int>());
        }
        return decrypt;
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
            try {
                cpp_int generator = findGenerator(prime);
                cpp_int privateKey = getRange(2, prime - 2, seed);
                cpp_int publicKey = powm(generator, privateKey, prime);
                std::cout << "Generator: " << MyPgp::toLittleEndianHex(generator) << "\n";
                std::cout << "Private key: " << MyPgp::toLittleEndianHex(privateKey) << "\n";
                std::cout << "Public key: " << MyPgp::toLittleEndianHex(publicKey) << std::endl;
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

    cpp_int ElGamal::getRange(const cpp_int &min, const cpp_int &max,
        const std::optional<unsigned int> &seed)
    {
        boost::random::mt19937 gen(seed ? seed.value() : std::random_device{}());
        boost::random::uniform_int_distribution<cpp_int> distrib(min, max);
        return distrib(gen);
    }

    std::vector<cpp_int> ElGamal::parseInfo(
        const std::vector<std::string> &infos, std::size_t nb)
    {
        std::vector<cpp_int> values;
  
        if (infos.size() != nb)
            throw MyPgpException("Wrong infos.");
        for (const auto &info : infos)
            values.push_back(MyPgp::fromLittleEndianHex(info));
        return values;
    }
}