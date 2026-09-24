/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** ${descriptor}
*/

#define __STDC_LIMIT_MACROS
#include <cstdint>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <iomanip>

#include "AES.hpp"

std::string toHex(const std::string &str)
{
    std::stringstream ss;
    for (unsigned char c : str)
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(c);
    return ss.str();
}

Test(AES, Boostrap_Test_One_Block)
{
    MyPgp::AES aes;
    std::string msg = "The Iron Throne.";
    std::string key = "game of thrones\n";

    std::string crypt = aes.encrypt(msg + msg, key, true);
    std::string cryptHex(toHex(crypt));
    cr_assert_str_eq(cryptHex.c_str(), "9c109c7de18f08884af1b6dd575a0402", "Check encrypt function, got %s", cryptHex.c_str());
    std::string decrypt = aes.decrypt(crypt, key, true);
    cr_assert_str_eq(decrypt.c_str(), msg.c_str(), "Check decrypt function, got %s", cryptHex.c_str());
}

Test(AES, Boostrap_Test_Two_Block)
{
    MyPgp::AES aes;
    std::string msg = "The Iron Throne.The Iron Throne.";
    std::string key = "game of thrones\n";

    std::string crypt = aes.encrypt(msg, key, false);
    std::string cryptHex(toHex(crypt));
    cr_assert_str_eq(cryptHex.c_str(), "9c109c7de18f08884af1b6dd575a04029c109c7de18f08884af1b6dd575a0402", "Check encrypt function, got %s", cryptHex.c_str());
    std::string decrypt = aes.decrypt(crypt, key, false);
    cr_assert_str_eq(decrypt.c_str(), msg.c_str(), "Check decrypt function, got %s", cryptHex.c_str());
}

Test(AES, Wrong_Key_Size_Encrypt)
{
    MyPgp::AES aes;
    std::string msg = "The Iron Throne.";
    std::string key = "game of thrones";

    try {
        std::string crypt = aes.encrypt(msg, key);
    } catch (MyPgp::MyPgpException &e) {
        cr_assert_str_eq(e.what(), "MyPgp Error: The key is not of the right size. Must be 16, 24 or 32 bytes.", "Check encrypt key size error handling: %s", e.what());
    }
}

Test(AES, Wrong_Key_Size_Decrypt)
{
    MyPgp::AES aes;
    std::string msg = "The Iron Throne.";
    std::string key = "game of thrones";

    try {
        std::string crypt = aes.decrypt(msg, key);
    } catch (MyPgp::MyPgpException &e) {
        cr_assert_str_eq(e.what(), "MyPgp Error: The key is not of the right size. Must be 16, 24 or 32 bytes.", "Check decrypt key size error handling: %s", e.what());
    }
}
