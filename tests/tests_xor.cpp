/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** tests_xor
*/

#define __STDC_LIMIT_MACROS
#include <cstdint>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <iomanip>
#include <sstream>

#include "Xor.hpp"
#include "Exception.hpp"

static std::string toHex(const std::string &str)
{
    std::stringstream ss;
    for (unsigned char c : str)
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(c);
    return ss.str();
}

Test(XOR, Block_Encrypt_Example)
{
    std::string msg = "You know nothing, Jon Snow";
    std::string key = toHex("What is dead may never die");

    std::string crypt = MyPgp::Xor::encrypt(msg, key, true);
    cr_assert_str_eq(crypt.c_str(), "20070f2700071c6a4449060a490515164e4e12190b190011063c", "Check encrypt function, got %s", crypt.c_str());
}

Test(XOR, Block_Decrypt_Example)
{
    std::string crypt = "20070f2700071c6a4449060a490515164e4e12190b190011063c";
    std::string key = toHex("What is dead may never die");

    std::string decrypt = MyPgp::Xor::decrypt(crypt, key, true);
    cr_assert_str_eq(decrypt.c_str(), "You know nothing, Jon Snow", "Check decrypt function, got %s", decrypt.c_str());
}

Test(XOR, Block_Round_Trip)
{
    std::string msg = "The Iron Throne.";
    std::string key = toHex("game of thrones!");

    std::string crypt = MyPgp::Xor::encrypt(msg, key, true);
    std::string decrypt = MyPgp::Xor::decrypt(crypt, key, true);
    cr_assert_str_eq(decrypt.c_str(), msg.c_str(), "Check round trip, got %s", decrypt.c_str());
}

Test(XOR, Repeating_Key_Round_Trip)
{
    std::string msg = "You know nothing, Jon Snow";
    std::string key = toHex("Stark");

    std::string crypt = MyPgp::Xor::encrypt(msg, key);
    std::string decrypt = MyPgp::Xor::decrypt(crypt, key);
    cr_assert_str_eq(decrypt.c_str(), msg.c_str(), "Check round trip, got %s", decrypt.c_str());
}

Test(XOR, Empty_Message)
{
    std::string key = toHex("What is dead may never die");

    cr_assert_str_eq(MyPgp::Xor::encrypt("", key, true).c_str(), "", "Check encrypt of empty message");
    cr_assert_str_eq(MyPgp::Xor::decrypt("", key, true).c_str(), "", "Check decrypt of empty message");
}

Test(XOR, Block_Wrong_Size_Encrypt)
{
    std::string msg = "You know nothing";
    std::string key = toHex("What is dead may never die");
    bool thrown = false;

    try {
        MyPgp::Xor::encrypt(msg, key, true);
    } catch (MyPgp::MyPgpException &e) {
        thrown = true;
        cr_assert_str_eq(e.what(), "MyPgp Error: The message and the symmetric key must be the same size", "Check encrypt size error handling: %s", e.what());
    }
    cr_assert(thrown, "Encrypt in block mode must throw when message and key sizes differ");
}

Test(XOR, Block_Wrong_Size_Decrypt)
{
    std::string crypt = "20070f2700071c6a";
    std::string key = toHex("What is dead may never die");
    bool thrown = false;

    try {
        MyPgp::Xor::decrypt(crypt, key, true);
    } catch (MyPgp::MyPgpException &e) {
        thrown = true;
        cr_assert_str_eq(e.what(), "MyPgp Error: The message and the symmetric key must be the same size", "Check decrypt size error handling: %s", e.what());
    }
    cr_assert(thrown, "Decrypt in block mode must throw when cipher and key sizes differ");
}
