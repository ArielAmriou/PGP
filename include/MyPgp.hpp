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

namespace MyPgp {

    constexpr int EPISUCCESS = 0;
    constexpr int EPIERROR = 84;

    constexpr const char *HELP_FILE = "public/help.txt";

    enum class CryptoSystem {
        XOR,
        AES,
        RSA,
        PGP_XOR,
        PGP_AES
    };

    enum class Mode {
        CIPHER,
        DECIPHER,
        GENERATE
    };

    class MyPgp {
    public:
        MyPgp(int ac, char **av);

        void run();

        static void displayHelp();

    private:
        void parseArgs(std::vector<std::string> args);
        void launch();
        void parseMsg();

        CryptoSystem _cryptoSystem;
        Mode _mode;
        bool _block = false;
        std::optional<std::string> _key;
        std::string _msg;
        std::optional<long long> _p;
        std::optional<long long> _q;
    };
};

#endif
