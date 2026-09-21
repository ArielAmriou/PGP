/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** tests_xor
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "Xor.hpp"

static void redirect_all_std()
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(Xor, encrypt_known_value)
{
    std::string cript = MyPgp::Xor::encrypt("A", "B");

    cr_assert(cript == "03");
}

Test(Xor, decrypt_known_value)
{
    std::string msg = MyPgp::Xor::decrypt("03", "B");

    cr_assert(msg == "A");
}

Test(Xor, encrypt_decrypt_round_trip_same_length_key)
{
    std::string msg = "Hello";
    std::string key = "abcde";
    std::string cript = MyPgp::Xor::encrypt(msg, key);
    std::string decrypted = MyPgp::Xor::decrypt(cript, key);

    cr_assert(decrypted == msg);
}

Test(Xor, encrypt_decrypt_round_trip_shorter_key)
{
    std::string msg = "Hello, World!";
    std::string key = "key";
    std::string cript = MyPgp::Xor::encrypt(msg, key);
    std::string decrypted = MyPgp::Xor::decrypt(cript, key);

    cr_assert(decrypted == msg);
}

Test(Xor, encrypt_decrypt_round_trip_longer_key)
{
    std::string msg = "Hi";
    std::string key = "averylongkey";
    std::string cript = MyPgp::Xor::encrypt(msg, key);
    std::string decrypted = MyPgp::Xor::decrypt(cript, key);

    cr_assert(decrypted == msg);
}

Test(Xor, encrypt_decrypt_round_trip_empty_message)
{
    std::string msg = "";
    std::string key = "key";
    std::string cript = MyPgp::Xor::encrypt(msg, key);
    std::string decrypted = MyPgp::Xor::decrypt(cript, key);

    cr_assert(cript == "");
    cr_assert(decrypted == msg);
}

Test(Xor, encrypt_produces_padding_when_key_does_not_divide_message)
{
    std::string msg = "Hello";
    std::string key = "key";
    std::string cript = MyPgp::Xor::encrypt(msg, key);

    cr_assert(cript == std::string("230015070a\0", 11));
}

Test(Xor, encrypt_with_same_key_gives_zero_bytes)
{
    std::string msg = "same";
    std::string cript = MyPgp::Xor::encrypt(msg, msg);

    cr_assert(cript == "00000000");
}

Test(Xor, decrypt_is_symmetric_with_encrypt_using_block_flag)
{
    std::string msg = "Block flag should not change behaviour";
    std::string key = "flagkey";
    std::string cript = MyPgp::Xor::encrypt(msg, key, true);
    std::string decrypted = MyPgp::Xor::decrypt(cript, key, true);

    cr_assert(decrypted == msg);
}
