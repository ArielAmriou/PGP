/*
** EPITECH PROJECT, 2026
** MyPgp
** File description:
** MyPgp
*/

#ifndef MYPGP_HPP
    #define MYPGP_HPP

    #include <optional>
    #include <string>
    #include <vector>
    #include <functional>
    #include <boost/multiprecision/cpp_int.hpp>
    #include "ACipher.hpp"
    #include "RSA.hpp"
    #include "Exception.hpp"

namespace MyPgp {

    using boost::multiprecision::cpp_int;

    constexpr int EPISUCCESS = 0;
    constexpr int EPIERROR = 84;

    constexpr const char *HELP_FILE = "public/help.txt";

    enum class CryptoSystem {
        XOR,
        AES,
        RSA,
        PGP_XOR,
        PGP_AES,
        ELGAMAL
    };

    enum class Mode {
        CIPHER,
        DECIPHER,
        GENERATE
    };

    class MyPgp : public ACipher {
    public:
        MyPgp(std::vector<std::string> args);

        void run();

        static void displayHelp();
        static std::string hexToStr(const std::string &hex, std::size_t wordSize = 0);
        static std::string strToHex(const std::string &str, std::size_t wordSize = 0);
        static std::string reorder(std::string s, std::size_t wordSize = 0);
        static cpp_int fromLittleEndianHex(const std::string &hex);
        static std::string toLittleEndianHex(const cpp_int &n, std::size_t byteLen = 0);
        static std::vector<std::string> splitString(const std::string &str, char delim);

        template<ACipher::CipherFn func>
        static const std::string encrypt(const std::string &msg, const std::string &key, const bool block = false)
        {
            auto split = splitString(key, ':');
            if (split.size() != 2)
                throw MyPgpException("Wrong args.");
            auto encryptMsg = func(msg, split[SYM_KEY], block);
            auto raw = hexToStr(split[SYM_KEY]);
            auto encryptKey = RSA::encrypt(std::string(raw.rbegin(), raw.rend()), split[ASYM_KEY], block);
            return std::string(encryptKey + "\n" + encryptMsg);
        }

        template<ACipher::CipherFn func>
        static const std::string decrypt(const std::string &msg, const std::string &key, const bool block = false)
        {
            auto split = splitString(key, ':');
            if (split.size() != 2)
                throw MyPgpException("Wrong args.");
            auto raw = RSA::decrypt(split[SYM_KEY], split[ASYM_KEY], block);
            auto decryptKey = strToHex(std::string(raw.rbegin(), raw.rend()));
            return func(msg, decryptKey, block);
        }

    private:

        enum CYPHER_INFO {
            SYM_KEY,
            ASYM_KEY,
        };

        struct CryptoEntry {
            std::string_view name;
            CryptoSystem system;
            ACipher::CipherFn encrypt;
            ACipher::CipherFn decrypt;
        };

        void parseArgs(std::vector<std::string> args);
        void launch();
        void parseMsg();
        void keyGen();

        CryptoSystem _cryptoSystem;
        Mode _mode;
        bool _block = false;
        std::optional<std::string> _key;
        std::string _msg;
        std::optional<std::string> _p;
        std::optional<std::string> _q;

        static const std::array<CryptoEntry, 6> CRYPTO_SYSTEMS;
    };
};

#endif
