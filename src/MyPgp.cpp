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
#include <algorithm>

#include "ArgsParser.hpp"
#include "Exception.hpp"
#include "MyPgp.hpp"

#include "Xor.hpp"
#include "AES.hpp"
#include <iostream>
#include <bits/stdc++.h>

namespace MyPgp {

    const std::array<MyPgp::CryptoEntry, 5> MyPgp::CRYPTO_SYSTEMS{{
        {"xor", CryptoSystem::XOR, &Xor::encrypt, &Xor::decrypt, true},
        {"aes", CryptoSystem::AES, &AES::encrypt, &AES::decrypt, true},
        {"rsa", CryptoSystem::RSA, nullptr, nullptr, true},
        {"pgp-xor", CryptoSystem::PGP_XOR, nullptr, nullptr, false},
        {"pgp-aes", CryptoSystem::PGP_AES, nullptr, nullptr, false},
    }};

    MyPgp::MyPgp(std::vector<std::string> args)
    {
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
            [&args](const auto &pair) { return pair.name == args.front(); });
        if (system == CRYPTO_SYSTEMS.end())
            throw Parser::ArgsParserError(
                args.front() + " is not a valid method");
        _cryptoSystem = system->system;
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
        auto system = std::find_if(CRYPTO_SYSTEMS.begin(), CRYPTO_SYSTEMS.end(),
            [this](const auto &pair) { return pair.system == this->_cryptoSystem; });
        try {
            if (_mode == Mode::CIPHER && system->encrypt && _key) {
                std::cout << system->encrypt(_msg, _key.value(), _block) << std::endl;
            } else if (_mode == Mode::DECIPHER && system->decrypt && _key) {
                std::cout << system->decrypt(_msg, _key.value(), _block) << std::endl;
            }
        } catch (MyPgpException &e) {
            throw e;
        }
    }

    void MyPgp::run()
    {
        try {
            launch();
        } catch (MyPgpException &e) {
            throw e;
        }
    }

    std::string MyPgp::reorder(std::string s, std::size_t wordSize)
    {
        if (wordSize == 0) {
            std::reverse(s.begin(), s.end());
            return s;
        }
        for (std::size_t i = 0; i + wordSize <= s.size(); i += wordSize)
            std::reverse(s.begin() + i, s.begin() + i + wordSize);
        return s;
    }

    std::string MyPgp::hexToStr(const std::string &hex, std::size_t wordSize)
    {
        if (hex.size() % 2)
            throw MyPgpException("This is not an hex number: '" + hex + "'.");
        for (auto c : hex)
            if (!std::isxdigit(static_cast<unsigned char>(c)))
                throw MyPgpException("This is not an hex number: '" + hex + "'.");
        std::string out;
        for (std::size_t i = 0; i + 1 < hex.size(); i += 2)
            out += static_cast<char>(std::stoi(hex.substr(i, 2), nullptr, 16));
        return reorder(out, wordSize);
    }

    std::string MyPgp::strToHex(const std::string &str, std::size_t wordSize)
    {
        std::stringstream ss;
        for (unsigned char c : reorder(str, wordSize))
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(c);
        return ss.str();
    }
}
