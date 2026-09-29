/*
** EPITECH PROJECT, 2026
** MyPgp
** File description:
** MyPgp
*/

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string_view>
#include <utility>
#include <unistd.h>

#include "ArgsParser.hpp"
#include "Exception.hpp"
#include "MyPgp.hpp"

#include "Xor.hpp"
#include <iostream>
#include <bits/stdc++.h>

namespace MyPgp {

    static constexpr std::array<std::pair<std::string_view, CryptoSystem>, 5>
        CRYPTO_SYSTEMS{{
            {"xor", CryptoSystem::XOR},
            {"aes", CryptoSystem::AES},
            {"rsa", CryptoSystem::RSA},
            {"pgp-xor", CryptoSystem::PGP_XOR},
            {"pgp-aes", CryptoSystem::PGP_AES},
        }};

    MyPgp::MyPgp(int ac, char **av)
    {
        std::vector<std::string> args(av + 1, av + ac);

        parseArgs(args);
        if (_mode != Mode::GENERATE)
            parseMsg();
    }

    void MyPgp::parseArgs(std::vector<std::string> args)
    {
        if (Parser::ArgsParser::isArg(args, "-h"))
            throw Parser::Help();

        _block = Parser::ArgsParser::isArg(args, "-b");

        bool cipher = Parser::ArgsParser::isArg(args, "-c");
        bool decipher = Parser::ArgsParser::isArg(args, "-d");
        bool generate =
            std::find(args.begin(), args.end(), "-g") != args.end();

        if (cipher + decipher + generate != 1)
            throw Parser::ArgsParserError(
                "mode must be exactly one of -c, -d or -g");

        if (cipher)
            _mode = Mode::CIPHER;
        if (decipher)
            _mode = Mode::DECIPHER;
        if (generate) {
            _mode = Mode::GENERATE;
            auto values = Parser::ArgsParser::getArgList<long long>(args, "-g");
            if (values.size() != 2)
                throw Parser::ArgsParserError(
                    "-g requires exactly two arguments P and Q");
            _p = values[0];
            _q = values[1];
        }

        if (args.empty())
            throw Parser::Help();

        auto system = std::find_if(CRYPTO_SYSTEMS.begin(), CRYPTO_SYSTEMS.end(),
            [&args](const auto &pair) { return pair.first == args.front(); });
        if (system == CRYPTO_SYSTEMS.end())
            throw Parser::ArgsParserError(
                args.front() + " is not a valid method");
        _cryptoSystem = system->second;
        args.erase(args.begin());

        if (_mode == Mode::GENERATE && _cryptoSystem != CryptoSystem::RSA)
            throw Parser::ArgsParserError("-g mode is only available for rsa");

        if (_mode == Mode::GENERATE && !args.empty())
            throw Parser::ArgsParserError(
                "key is incompatible with -g mode");

        if (_mode != Mode::GENERATE) {
            if (args.empty())
                throw Parser::ArgsParserError(
                    "key is mandatory for -c and -d mode");
            _key = args.front();
            args.erase(args.begin());
        }

        if (!args.empty())
            throw Parser::ArgsParserError("too many arguments");
    }

    void MyPgp::parseMsg()
    {
        std::string line;

        while (std::getline(std::cin, line)) { 
            _msg += line;
            if (isatty(STDIN_FILENO))
                return;
            _msg += '\n';
        }
        _msg.pop_back();
    }

    void MyPgp::displayHelp()
    {
        std::ifstream file(HELP_FILE);

        if (!file.is_open())
            throw MyPgpException("unable to open help file");
        std::cout << file.rdbuf();
    }

    void MyPgp::launch()
    {
        
    }

    void MyPgp::run()
    {
        try {
            launch();
        } catch (MyPgpException &e) {
            throw e;
        }
    }

}
