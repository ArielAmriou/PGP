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
    #include <gmpxx.h>
    #include "ACipher.hpp"
    #include "RSA.hpp"
    #include "Exception.hpp"

namespace MyPgp {

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

    class MyPgp {
    public:
        MyPgp(std::vector<std::string> args);

        void run();

        static void displayHelp();
        static bool isHex(const std::string &str);
        static std::string hexToStr(const std::string &hex, std::size_t wordSize = 0);
        static std::string strToHex(const std::string &str, std::size_t wordSize = 0);
        static std::string reorder(std::string s, std::size_t wordSize = 0);
        static mpz_class fromLittleEndianHex(const std::string &hex);
        static std::string toLittleEndianHex(const mpz_class &n, std::size_t byteLen = 0);
        static std::vector<std::string> splitString(const std::string &str, char delim);
        static mpz_class powm(const mpz_class &base, const mpz_class &exp, const mpz_class &mod);
        static mpz_class modInverse(const mpz_class &a, const mpz_class &m);
        static std::size_t msb(const mpz_class &n);

    private:

        struct CryptoEntry {
            std::string_view name;
            CryptoSystem system;
            ACipher::CipherFn encrypt;
            ACipher::CipherFn decrypt;
            ACipher::KeyCheckFn checkKey;
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
        const CryptoEntry *_system = nullptr;

        static const std::array<CryptoEntry, 6> CRYPTO_SYSTEMS;
    };
};

#endif
