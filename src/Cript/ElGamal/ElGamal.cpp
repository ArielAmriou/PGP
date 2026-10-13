/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** ElGamal
*/

#include <iostream>
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
            seed = static_cast<unsigned int>(MyPgp::fromLittleEndianHex(args[1]).get_ui());
        mpz_class tmpKey = getRange(2, infos[ENCRYPT_PRIME], seed);
        mpz_class c1 = MyPgp::powm(infos[ENCRYPT_GENERATOR], tmpKey, infos[ENCRYPT_PRIME]);
        std::string crypt = MyPgp::toLittleEndianHex(c1) + "-";
        mpz_class secret = MyPgp::powm(infos[ENCRYPT_PUBLIC], tmpKey, infos[ENCRYPT_PRIME]);
        std::size_t byteLen = MyPgp::msb(infos[ENCRYPT_PRIME] - 1) / BYTESIZE + 1;
        for (unsigned char ch : msg) {
            mpz_class m = ch;
            if (m >= infos[ENCRYPT_PRIME])
                throw MyPgpException("Prime too small for this message.");
            mpz_class c2 = (m * secret) % infos[ENCRYPT_PRIME];
            crypt += MyPgp::toLittleEndianHex(c2, byteLen);
            if (block)
                break;
        }
        return crypt;
    }
        
    const std::string ElGamal::decrypt(const std::string &cript, const std::string &key, const bool block)
    {
        auto msgInfos = MyPgp::splitString(cript, '-');
        auto keyInfos = parseInfo(MyPgp::splitString(key, '-'), NB_DECRYPT_KEY_INFO);
        if (msgInfos.size() != NB_DECRYPT_MSG_INFO)
            throw MyPgpException("Wrong infos.");
        mpz_class c1 = MyPgp::fromLittleEndianHex(msgInfos[DECRYPT_C1]);
        mpz_class secret = MyPgp::powm(c1, keyInfos[DECRYPT_PRIVATE], keyInfos[DECRYPT_PRIME]);
        mpz_class invSecret = MyPgp::powm(secret, keyInfos[DECRYPT_PRIME] - 2, keyInfos[DECRYPT_PRIME]);
        std::size_t hexLen = (MyPgp::msb(keyInfos[DECRYPT_PRIME] - 1) / BYTESIZE + 1) * 2;
        std::string decrypt;
        std::string body(msgInfos[DECRYPT_C2]);
        if (body.size() % hexLen != 0)
            throw MyPgpException("Wrong message length.");
        for (std::size_t i = 0; i < body.size(); i += hexLen) {
            mpz_class c2 = MyPgp::fromLittleEndianHex(body.substr(i, hexLen));
            mpz_class m = (c2 * invSecret) % keyInfos[DECRYPT_PRIME];
            if (m > 255)
                throw MyPgpException("Decryption failed.");
            decrypt += static_cast<char>(m.get_si());
            if (block)
                break;
        }
        return decrypt;
    }

    void ElGamal::keyGen(const std::string &p)
    {
            auto args = MyPgp::splitString(p, ':');

            if (args.empty() || args.size() > 2)
                throw MyPgpException("Wrong args.");
            mpz_class prime = MyPgp::fromLittleEndianHex(args[0]);
            std::optional<unsigned int> seed;
            if (args.size() == 2)
                seed = static_cast<unsigned int>(MyPgp::fromLittleEndianHex(args[1]).get_ui());
            try {
                mpz_class generator = findGenerator(prime);
                mpz_class privateKey = getRange(2, prime - 2, seed);
                mpz_class publicKey = MyPgp::powm(generator, privateKey, prime);
                std::cout << "Generator: " << MyPgp::toLittleEndianHex(generator) << "\n";
                std::cout << "Private key: " << MyPgp::toLittleEndianHex(privateKey) << "\n";
                std::cout << "Public key: " << MyPgp::toLittleEndianHex(publicKey) << std::endl;
            } catch (MyPgpException &e) {
                throw e;
            }
    }

    mpz_class ElGamal::findGenerator(const mpz_class &p)
    {
        mpz_class q = (p - 1) / 2;

        for (mpz_class g = 2; g < p - 1; ++g) {
            if (MyPgp::powm(g, mpz_class(2), p) != 1 && MyPgp::powm(g, q, p) != 1)
                return g;
        }
        throw MyPgpException("No generator found");
    }

    mpz_class ElGamal::getRange(const mpz_class &min, const mpz_class &max,
        const std::optional<unsigned int> &seed)
    {
        gmp_randstate_t state;
        gmp_randinit_mt(state);
        gmp_randseed_ui(state, seed ? seed.value() : std::random_device{}());

        mpz_class range = max - min + 1;
        mpz_class result;
        mpz_urandomm(result.get_mpz_t(), state, range.get_mpz_t());
        result += min;

        gmp_randclear(state);
        return result;
    }

    std::vector<mpz_class> ElGamal::parseInfo(
        const std::vector<std::string> &infos, std::size_t nb)
    {
        std::vector<mpz_class> values;

        if (infos.size() != nb)
            throw MyPgpException("Wrong infos.");
        for (const auto &info : infos)
            values.push_back(MyPgp::fromLittleEndianHex(info));
        return values;
    }

    void ElGamal::checkKey(const std::string &key, const Mode mode)
    {
        if (mode == Mode::GENERATE) {
            auto args = MyPgp::splitString(key, ':');
            if (args.empty() || args.size() > 2)
                throw MyPgpException("Invalid key format");
            for (const auto &arg : args)
                if (!MyPgp::isHex(arg))
                    throw MyPgpException("Invalid key format");
            return;
        }
        if (mode == Mode::DECIPHER) {
            auto parts = MyPgp::splitString(key, '-');
            if (parts.size() != NB_DECRYPT_KEY_INFO)
                throw MyPgpException("Invalid key format");
            for (const auto &part : parts)
                if (!MyPgp::isHex(part))
                    throw MyPgpException("Invalid key format");
            return;
        }
        auto args = MyPgp::splitString(key, ':');
        if (args.empty() || args.size() > 2)
            throw MyPgpException("Invalid key format");
        if (args.size() == 2 && !MyPgp::isHex(args[1]))
            throw MyPgpException("Invalid key format");
        auto parts = MyPgp::splitString(args[0], '-');
        if (parts.size() != NB_ENCRYPT_INFO)
            throw MyPgpException("Invalid key format");
        for (const auto &part : parts)
            if (!MyPgp::isHex(part))
                throw MyPgpException("Invalid key format");
    }
}