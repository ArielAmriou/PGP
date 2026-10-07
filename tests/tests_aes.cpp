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
#include "MyPgp.hpp"

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

    std::string crypt = aes.basicEncrypt(msg + msg, key, true);
    std::string cryptHex(toHex(crypt));
    cr_assert_str_eq(cryptHex.c_str(), "9c109c7de18f08884af1b6dd575a0402", "Check encrypt function, got %s", cryptHex.c_str());
    std::string decrypt = aes.basicDecrypt(crypt, key, true);
    cr_assert_str_eq(decrypt.c_str(), msg.c_str(), "Check decrypt function, got %s", cryptHex.c_str());
}

Test(AES, Boostrap_Test_Two_Block)
{
    MyPgp::AES aes;
    std::string msg = "The Iron Throne.The Iron Throne.";
    std::string key = "game of thrones\n";

    std::string crypt = aes.basicEncrypt(msg, key, false);
    std::string cryptHex(toHex(crypt));
    cr_assert_str_eq(cryptHex.c_str(), "9c109c7de18f08884af1b6dd575a04029c109c7de18f08884af1b6dd575a0402", "Check encrypt function, got %s", cryptHex.c_str());
    std::string decrypt = aes.basicDecrypt(crypt, key, false);
    cr_assert_str_eq(decrypt.c_str(), msg.c_str(), "Check decrypt function, got %s", cryptHex.c_str());
}

Test(AES, Wrong_Key_Size_Encrypt)
{
    MyPgp::AES aes;
    std::string msg = "The Iron Throne.";
    std::string key = "game of thrones";

    try {
        std::string crypt = aes.basicEncrypt(msg, key);
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
        std::string crypt = aes.basicDecrypt(msg, key);
    } catch (MyPgp::MyPgpException &e) {
        cr_assert_str_eq(e.what(), "MyPgp Error: The key is not of the right size. Must be 16, 24 or 32 bytes.", "Check decrypt key size error handling: %s", e.what());
    }
}

Test(AES_CheckKey, accepts_16_byte_key)
{
    try {
        MyPgp::AES::checkKey("000102030405060708090a0b0c0d0e0f", MyPgp::Mode::CIPHER);
    } catch (MyPgp::MyPgpException &e) {
        cr_assert_fail("A valid 16 byte key must not be rejected: %s", e.what());
    }
}

Test(AES_CheckKey, accepts_24_byte_key)
{
    try {
        MyPgp::AES::checkKey("000102030405060708090a0b0c0d0e0f1011121314151617", MyPgp::Mode::CIPHER);
    } catch (MyPgp::MyPgpException &e) {
        cr_assert_fail("A valid 24 byte key must not be rejected: %s", e.what());
    }
}

Test(AES_CheckKey, accepts_32_byte_key)
{
    try {
        MyPgp::AES::checkKey("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f", MyPgp::Mode::CIPHER);
    } catch (MyPgp::MyPgpException &e) {
        cr_assert_fail("A valid 32 byte key must not be rejected: %s", e.what());
    }
}

Test(AES_CheckKey, rejects_wrong_key_size)
{
    try {
        MyPgp::AES::checkKey("000102030405060708090a0b0c0d0e", MyPgp::Mode::CIPHER);
        cr_assert_fail("A 15 byte key must be rejected");
    } catch (MyPgp::MyPgpException &e) {
        cr_assert_str_eq(e.what(), "MyPgp Error: Invalid key format", "Got: %s", e.what());
    }
}

Test(AES_CheckKey, rejects_non_hex_key)
{
    try {
        MyPgp::AES::checkKey("gg0102030405060708090a0b0c0d0e0f", MyPgp::Mode::CIPHER);
        cr_assert_fail("A non hexadecimal key must be rejected");
    } catch (MyPgp::MyPgpException &e) {
        cr_assert_str_eq(e.what(), "MyPgp Error: Invalid key format", "Got: %s", e.what());
    }
}

Test(AES_CheckKey, rejects_odd_length_key)
{
    try {
        MyPgp::AES::checkKey("000102030405060708090a0b0c0d0e0f0", MyPgp::Mode::CIPHER);
        cr_assert_fail("An odd length key must be rejected");
    } catch (MyPgp::MyPgpException &e) {
        cr_assert_str_eq(e.what(), "MyPgp Error: Invalid key format", "Got: %s", e.what());
    }
}

Test(AES_CheckKey, rejects_empty_key)
{
    try {
        MyPgp::AES::checkKey("", MyPgp::Mode::CIPHER);
        cr_assert_fail("An empty key must be rejected");
    } catch (MyPgp::MyPgpException &e) {
        cr_assert_str_eq(e.what(), "MyPgp Error: Invalid key format", "Got: %s", e.what());
    }
}
