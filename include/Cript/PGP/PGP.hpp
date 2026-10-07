/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** PGP
*/

#ifndef PGP_HPP_
#define PGP_HPP_

#include "ACipher.hpp"
#include "MyPgp.hpp"

namespace MyPgp {
    
    class PGP : public ACipher {
    public:
        template<ACipher::CipherFn func>
        static const std::string encrypt(const std::string &msg, const std::string &key, const bool block = false)
        {
            auto split = MyPgp::splitString(key, ':');
            if (split.size() != 2)
                throw MyPgpException("Wrong args.");
            auto encryptMsg = func(msg, split[SYM_KEY], block);
            auto raw = MyPgp::hexToStr(split[SYM_KEY]);
            auto encryptKey = RSA::encrypt(std::string(raw.rbegin(), raw.rend()), split[ASYM_KEY], block);
            return std::string(encryptKey + "\n" + encryptMsg);
        }

        template<ACipher::CipherFn func>
        static const std::string decrypt(const std::string &msg, const std::string &key, const bool block = false)
        {
            auto split = MyPgp::splitString(key, ':');
            if (split.size() != 2)
                throw MyPgpException("Wrong args.");
            auto raw = RSA::decrypt(split[SYM_KEY], split[ASYM_KEY], block);
            auto decryptKey = MyPgp::strToHex(std::string(raw.rbegin(), raw.rend()));
            return func(msg, decryptKey, block);
        }

        template<ACipher::KeyCheckFn symCheck, ACipher::KeyCheckFn asymCheck>
        static void checkKey(const std::string &key, const Mode mode)
        {
            auto split = MyPgp::splitString(key, ':');
            if (split.size() != 2)
                throw MyPgpException("Wrong args.");
            asymCheck(split[ASYM_KEY], mode);
            if (mode == Mode::CIPHER)
                symCheck(split[SYM_KEY], mode);
        }

    private:
        
        enum CYPHER_INFO {
            SYM_KEY,
            ASYM_KEY,
        };
    };
}

#endif /* !PGP_HPP_ */
