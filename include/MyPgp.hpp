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
    #include <boost/multiprecision/cpp_int.hpp>
    #include "ACipher.hpp"

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

    class MyPgp {
    public:
        MyPgp(std::vector<std::string> args);

        void run();

        static void displayHelp();
        static std::string hexToStr(const std::string &hex, std::size_t wordSize = 0);
        static std::string strToHex(const std::string &str, std::size_t wordSize = 0);
        static std::string reorder(std::string s, std::size_t wordSize = 0);
        static cpp_int fromLittleEndianHex(const std::string &hex);
        static std::string toLittleEndianHex(const cpp_int &n);
        static std::vector<std::string> splitString(const std::string &str, char delim);

    private:

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
