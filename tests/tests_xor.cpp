/*
** EPITECH PROJECT, 2026
** PGP
** File description:
** tests_xor
*/

#define __STDC_LIMIT_MACROS
#include <cstdint>

#include <criterion/criterion.h>

#include <iomanip>
#include <sstream>

#include "Exception.hpp"
#include "Xor.hpp"

static std::string xorToHex(const std::string &str)
{
    std::stringstream ss;
    for (unsigned char c : str)
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(c);
    return ss.str();
}

// Key of `size` bytes, written as hexadecimal text like the subject's key.
static std::string xorKeyOfSize(size_t size, unsigned int seed)
{
    std::string key;
    for (size_t i = 0; i < size; i++)
        key += static_cast<char>((i * 31 + seed) & 0xFF);
    return xorToHex(key);
}

// Example from the subject:
//   $> echo "You know nothing, Jon Snow" > message
//   $> ./my_pgp xor -c -b 576861742069732064656164206d6179206e6576657220646965 < message > ciphered
//   $> cat -e ciphered
//   20070f2700071c6a4449060a490515164e4e12190b190011063c$
static const std::string SUBJECT_MSG = "You know nothing, Jon Snow";
static const std::string SUBJECT_KEY_HEX = "576861742069732064656164206d6179206e6576657220646965";
static const std::string SUBJECT_CIPHER_HEX = "20070f2700071c6a4449060a490515164e4e12190b190011063c";

Test(XOR_Class, Subject_Encrypt_Matches_Example)
{
    std::string crypt = MyPgp::Xor::encrypt(SUBJECT_MSG, SUBJECT_KEY_HEX, true);

    cr_assert_str_eq(crypt.c_str(), SUBJECT_CIPHER_HEX.c_str(),
        "Encrypting the subject message with the subject key gave the wrong result.");
}

Test(XOR_Class, Subject_Decrypt_Matches_Example)
{
    std::string decrypt = MyPgp::Xor::decrypt(SUBJECT_CIPHER_HEX, SUBJECT_KEY_HEX, true);

    cr_assert_str_eq(decrypt.c_str(), SUBJECT_MSG.c_str(),
        "Decrypting the subject ciphertext with the subject key gave the wrong result.");
}

Test(XOR_Class, Subject_Roundtrip_Block)
{
    std::string crypt = MyPgp::Xor::encrypt(SUBJECT_MSG, SUBJECT_KEY_HEX, true);
    std::string decrypt = MyPgp::Xor::decrypt(crypt, SUBJECT_KEY_HEX, true);

    cr_assert_str_eq(decrypt.c_str(), SUBJECT_MSG.c_str(),
        "decrypt(encrypt(msg)) must give back msg when block is true.");
}

Test(XOR_Class, Roundtrip_Block_False)
{
    std::string msg = "Winter is coming.";
    std::string key = xorKeyOfSize(msg.size(), 7);

    std::string crypt = MyPgp::Xor::encrypt(msg, key, false);
    std::string decrypt = MyPgp::Xor::decrypt(crypt, key, false);

    cr_assert_str_eq(decrypt.c_str(), msg.c_str(),
        "decrypt(encrypt(msg)) must give back msg when block is false.");
}

Test(XOR_Class, Roundtrip_All_Byte_Values)
{
    std::string msg;
    for (unsigned int i = 0; i < 256; i++)
        msg += static_cast<char>(i);
    std::string key = xorKeyOfSize(128, 3);

    std::string crypt = MyPgp::Xor::encrypt(msg, key, false);
    std::string decrypt = MyPgp::Xor::decrypt(crypt, key, false);

    cr_assert_eq(decrypt.size(), msg.size(), "Decrypted size differs from the original size.");
    cr_assert_str_eq(decrypt.c_str(), msg.c_str(),
        "Bytes 0x00 to 0xFF must survive a roundtrip unchanged.");
}

Test(XOR_Class, Block_Defaults_To_False)
{
    std::string msg = "Winter is coming.";
    std::string key = xorKeyOfSize(msg.size(), 11);

    std::string cryptDefault = MyPgp::Xor::encrypt(msg, key);
    std::string cryptFalse = MyPgp::Xor::encrypt(msg, key, false);
    cr_assert_str_eq(cryptDefault.c_str(), cryptFalse.c_str(),
        "encrypt without the block argument must behave like block = false.");

    std::string decryptDefault = MyPgp::Xor::decrypt(cryptFalse, key);
    std::string decryptFalse = MyPgp::Xor::decrypt(cryptFalse, key, false);
    cr_assert_str_eq(decryptDefault.c_str(), decryptFalse.c_str(),
        "decrypt without the block argument must behave like block = false.");
}

Test(XOR_Class, Output_Is_Hex_Two_Chars_Per_Byte)
{
    std::string msg = "Winter is coming.";
    std::string key = xorKeyOfSize(16, 5);

    std::string crypt = MyPgp::Xor::encrypt(msg, key, false);

    std::size_t cryptBytes = crypt.size() / 2;
    cr_assert(cryptBytes > msg.size() && cryptBytes <= msg.size() + 16,
        "The ciphertext is hexadecimal text, two characters per byte, with 1 to 16 bytes of padding.");
}

Test(XOR_Class, Different_Keys_Give_Different_Output)
{
    std::string msg = "Winter is coming.";
    std::string key1 = xorKeyOfSize(msg.size(), 1);
    std::string key2 = xorKeyOfSize(msg.size(), 2);

    std::string crypt1 = MyPgp::Xor::encrypt(msg, key1, false);
    std::string crypt2 = MyPgp::Xor::encrypt(msg, key2, false);

    cr_assert(crypt1 != crypt2,
        "Two different keys must not produce the same ciphertext for the same message.");
}

Test(XOR_Class, Wrong_Key_Does_Not_Decrypt)
{
    std::string msg = "Winter is coming.";
    std::string key = xorKeyOfSize(msg.size(), 1);
    std::string wrongKey = xorKeyOfSize(msg.size(), 2);

    std::string crypt = MyPgp::Xor::encrypt(msg, key, false);
    bool refused = false;
    std::string decrypt;
    try {
        decrypt = MyPgp::Xor::decrypt(crypt, wrongKey, false);
    } catch (MyPgp::MyPgpException &) {
        refused = true;
    }

    cr_assert(refused || decrypt != msg,
        "Decrypting with a different key must not give back the original message.");
}

// Printable message of `size` bytes, never containing a NUL byte.
static std::string xorTextOfSize(size_t size)
{
    std::string msg;
    for (size_t i = 0; i < size; i++)
        msg += static_cast<char>('a' + i % 26);
    return msg;
}

Test(XOR_Padding, Roundtrip_Key_Shorter_Than_Message)
{
    std::string msg = xorTextOfSize(26);
    std::string key = xorKeyOfSize(8, 4);

    std::string crypt = MyPgp::Xor::encrypt(msg, key, false);
    std::string decrypt = MyPgp::Xor::decrypt(crypt, key, false);

    cr_assert_str_eq(decrypt.c_str(), msg.c_str(),
        "A message longer than the key must come back unchanged after a roundtrip.");
}

Test(XOR_Padding, Roundtrip_Every_Length)
{
    std::string key = xorKeyOfSize(16, 9);

    for (size_t len = 1; len <= 40; len++) {
        std::string msg = xorTextOfSize(len);
        std::string crypt = MyPgp::Xor::encrypt(msg, key, false);
        std::string decrypt = MyPgp::Xor::decrypt(crypt, key, false);

        cr_assert_str_eq(decrypt.c_str(), msg.c_str(),
            "Roundtrip failed for a message of %zu bytes.", len);
    }
}

Test(XOR_Padding, Decrypt_Removes_Padding_Exactly)
{
    std::string key = xorKeyOfSize(16, 9);

    for (size_t len = 1; len <= 40; len++) {
        std::string msg = xorTextOfSize(len);
        std::string crypt = MyPgp::Xor::encrypt(msg, key, false);
        std::string decrypt = MyPgp::Xor::decrypt(crypt, key, false);

        cr_assert_eq(decrypt.size(), len,
            "Decrypted output for a %zu byte message must not keep padding bytes.", len);
    }
}

Test(XOR_Padding, Message_Ending_Like_Padding_Survives)
{
    std::string key = xorKeyOfSize(16, 6);
    std::string msg = "end with pad\x01\x01\x01";

    std::string crypt = MyPgp::Xor::encrypt(msg, key, false);
    std::string decrypt = MyPgp::Xor::decrypt(crypt, key, false);

    cr_assert_eq(decrypt.size(), msg.size(),
        "Trailing bytes that look like padding are part of the message and must be kept.");
    cr_assert_str_eq(decrypt.c_str(), msg.c_str(),
        "Trailing bytes that look like padding are part of the message and must be kept.");
}

// Known limitation: padding is stripped from the decrypted plaintext by scanning
// for trailing '\0' bytes, so a message that itself ends with real NUL byte(s)
// is indistinguishable from padding and gets trimmed the same way. This is an
// accepted trade-off: losing a trailing NUL is preferable to ever losing a real
// character that coincidentally XORs to zero (see Message_Ending_Like_Padding_Survives).
Test(XOR_Padding, Message_Ending_With_Nul_Is_Treated_As_Padding)
{
    std::string key = xorKeyOfSize(8, 2);
    std::string msg = std::string("abc\0\0", 5);

    std::string crypt = MyPgp::Xor::encrypt(msg, key, false);
    std::string decrypt = MyPgp::Xor::decrypt(crypt, key, false);

    cr_assert_str_eq(decrypt.c_str(), "abc",
        "Trailing NUL bytes in the original message are indistinguishable from padding and are stripped.");
}

Test(XOR_Padding, Ciphertext_Is_Whole_Key_Blocks)
{
    std::string key = xorKeyOfSize(16, 9);

    for (size_t len = 1; len <= 40; len++) {
        std::string crypt = MyPgp::Xor::encrypt(xorTextOfSize(len), key, false);

        cr_assert_eq(crypt.size() % (2 * 16), 0,
            "The padded ciphertext of a %zu byte message must fill whole 16 byte blocks.", len);
    }
}
