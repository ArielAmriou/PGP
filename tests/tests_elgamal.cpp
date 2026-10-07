/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** Unit tests for ElGamal
*/

#include <criterion/criterion.h>

#include <cstring>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "ElGamal.hpp"
#include "Exception.hpp"
#include "MyPgp.hpp"

using boost::multiprecision::cpp_int;
using boost::multiprecision::powm;

namespace {

    const std::string SMALL_P    = "d301";
    const std::string SMALL_G    = "02";
    const std::string SMALL_PRIV = "9900";
    const std::string SMALL_PUB  = "e000";
    const std::string SMALL_C1   = "bd00";
    const std::string SMALL_HI   = "bd00-9101fe00";
    const std::string SMALL_BAD  = "bd00-c000";

    const std::string BIG_P =
        "671f8577d48b8e9c3140adb32569005ee53226fdf215ff8d63160df745333caa"
        "584b3ab46c7a7e854415d018454dea1ecf55d6c63c3191fbc7b152e87a145f8b";

    struct KeyPair {
        cpp_int generator;
        cpp_int priv;
        cpp_int pub;
    };

    KeyPair runKeyGen(const std::string &arg)
    {
        std::ostringstream out;
        std::streambuf *old = std::cout.rdbuf(out.rdbuf());

        try {
            MyPgp::ElGamal::keyGen(arg);
        } catch (...) {
            std::cout.rdbuf(old);
            throw;
        }
        std::cout.rdbuf(old);

        std::istringstream in(out.str());
        std::string line;
        KeyPair kp;
        int found = 0;
        while (std::getline(in, line)) {
            auto pos = line.find(": ");
            if (pos == std::string::npos)
                continue;
            std::string name = line.substr(0, pos);
            cpp_int val = MyPgp::MyPgp::MyPgp::fromLittleEndianHex(line.substr(pos + 2));
            if (name == "Generator") { kp.generator = val; found++; }
            else if (name == "Private key") { kp.priv = val; found++; }
            else if (name == "Public key") { kp.pub = val; found++; }
        }
        cr_assert_eq(found, 3, "keyGen must print Generator, Private key and Public key, got: %s", out.str().c_str());
        return kp;
    }

    template <typename F>
    void assertMyPgpError(F fn, const char *expected)
    {
        try {
            fn();
        } catch (MyPgp::MyPgpException &e) {
            cr_assert(std::strstr(e.what(), expected) != nullptr,
                "Expected error containing \"%s\", got \"%s\"", expected, e.what());
            return;
        } catch (...) {
            cr_assert_fail("Expected a MyPgpException, got another exception type");
        }
        cr_assert_fail("Expected a MyPgpException containing \"%s\", but nothing was thrown", expected);
    }

    std::string makePublicKey(const std::string &p, const std::string &g, const std::string &pub)
    {
        return p + "-" + g + "-" + pub;
    }
}

/* ------------------------------------------------------------------ */
/* keyGen                                                              */
/* ------------------------------------------------------------------ */

Test(ElGamal_KeyGen, small_prime_generator_is_found)
{
    KeyPair kp = runKeyGen(SMALL_P + ":01");

    cr_assert(kp.generator == 2, "Expected generator 2 for p = 467, got %s", kp.generator.str().c_str());
}

Test(ElGamal_KeyGen, private_key_is_in_range)
{
    cpp_int p = MyPgp::MyPgp::fromLittleEndianHex(SMALL_P);

    for (int seed = 1; seed < 50; seed++) {
        std::string hexSeed = MyPgp::MyPgp::toLittleEndianHex(cpp_int(seed), 1);
        KeyPair kp = runKeyGen(SMALL_P + ":" + hexSeed);
        cr_assert(kp.priv >= 2 && kp.priv <= p - 2, "Private key %s out of [2, p-2]", kp.priv.str().c_str());
    }
}

Test(ElGamal_KeyGen, public_key_is_generator_power_private_key)
{
    cpp_int p = MyPgp::MyPgp::fromLittleEndianHex(BIG_P);
    KeyPair kp = runKeyGen(BIG_P + ":34");

    cr_assert(kp.pub == powm(kp.generator, kp.priv, p), "Public key must be g^x mod p");
    cr_assert(kp.pub < p, "Public key must be smaller than the prime");
    cr_assert(kp.priv < p, "Private key must be smaller than the prime");
}

Test(ElGamal_KeyGen, same_seed_gives_same_keys)
{
    KeyPair a = runKeyGen(BIG_P + ":34");
    KeyPair b = runKeyGen(BIG_P + ":34");

    cr_assert(a.generator == b.generator);
    cr_assert(a.priv == b.priv, "Same seed must give the same private key");
    cr_assert(a.pub == b.pub, "Same seed must give the same public key");
}

Test(ElGamal_KeyGen, different_seed_gives_different_keys)
{
    KeyPair a = runKeyGen(BIG_P + ":34");
    KeyPair b = runKeyGen(BIG_P + ":35");

    cr_assert(a.priv != b.priv, "Different seeds should give different private keys");
}

Test(ElGamal_KeyGen, no_seed_gives_random_keys)
{
    KeyPair a = runKeyGen(BIG_P);
    KeyPair b = runKeyGen(BIG_P);

    cr_assert(a.priv != b.priv, "Two unseeded runs should not give the same private key");
}

Test(ElGamal_KeyGen, generator_is_never_a_quadratic_residue_on_safe_prime)
{
    cpp_int p = MyPgp::MyPgp::fromLittleEndianHex(BIG_P);
    cpp_int q = (p - 1) / 2;
    KeyPair kp = runKeyGen(BIG_P + ":34");

    cr_assert(kp.generator != 2, "2 is a quadratic residue for this prime, it cannot be a generator");
    cr_assert(powm(kp.generator, cpp_int(2), p) != 1, "g^2 must not be 1");
    cr_assert(powm(kp.generator, q, p) != 1, "g^q must not be 1");
}

Test(ElGamal_KeyGen, too_many_args)
{
    assertMyPgpError([] { MyPgp::ElGamal::keyGen(SMALL_P + ":01:02"); }, "Wrong args.");
}

Test(ElGamal_KeyGen, prime_too_small_has_no_generator)
{
    assertMyPgpError([] { MyPgp::ElGamal::keyGen("02"); }, "No generator found");
}

Test(ElGamal_Encrypt, output_format)
{
    std::string key = makePublicKey(SMALL_P, SMALL_G, SMALL_PUB) + ":05";
    std::string msg = "Hello";
    std::string crypt = MyPgp::ElGamal::encrypt(msg, key);

    auto pos = crypt.find('-');
    cr_assert(pos != std::string::npos, "Ciphertext must be \"c1-c2\", got %s", crypt.c_str());
    cr_assert_eq(crypt.find('-', pos + 1), std::string::npos, "Only one '-' expected");
    cr_assert_eq(crypt.size() - pos - 1, msg.size() * 4, "Each char must be encrypted on 2 bytes");
}

Test(ElGamal_Encrypt, same_seed_same_ciphertext)
{
    std::string key = makePublicKey(BIG_P, "03", "05") + ":2a";

    cr_assert_str_eq(MyPgp::ElGamal::encrypt("Hello", key).c_str(),
                     MyPgp::ElGamal::encrypt("Hello", key).c_str(),
                     "Same seed must give the same ciphertext");
}

Test(ElGamal_Encrypt, block_mode_encrypts_only_first_char)
{
    std::string key = makePublicKey(SMALL_P, SMALL_G, SMALL_PUB) + ":05";
    std::string crypt = MyPgp::ElGamal::encrypt("Hello", key, true);
    auto pos = crypt.find('-');

    cr_assert_eq(crypt.size() - pos - 1, 4, "Block mode must only output one encrypted char");
}

Test(ElGamal_Encrypt, empty_message_only_outputs_c1)
{
    std::string key = makePublicKey(SMALL_P, SMALL_G, SMALL_PUB) + ":05";
    std::string crypt = MyPgp::ElGamal::encrypt("", key);

    cr_assert_eq(crypt.back(), '-', "With an empty message only \"c1-\" is expected, got %s", crypt.c_str());
}

Test(ElGamal_Encrypt, wrong_number_of_key_parts)
{
    assertMyPgpError([] { MyPgp::ElGamal::encrypt("Hi", SMALL_P + "-" + SMALL_G); }, "Wrong infos.");
    assertMyPgpError([] { MyPgp::ElGamal::encrypt("Hi", SMALL_P + "-" + SMALL_G + "-" + SMALL_PUB + "-00"); }, "Wrong infos.");
}

Test(ElGamal_Encrypt, too_many_seed_args)
{
    assertMyPgpError([] {
        MyPgp::ElGamal::encrypt("Hi", makePublicKey(SMALL_P, SMALL_G, SMALL_PUB) + ":01:02");
    }, "Wrong args.");
}

Test(ElGamal_Encrypt, prime_too_small_for_message)
{
    assertMyPgpError([] {
        MyPgp::ElGamal::encrypt("A", makePublicKey("17", "05", "01") + ":01");
    }, "Prime too small for this message.");
}

Test(ElGamal_Decrypt, known_answer)
{
    std::string key = makePublicKey(SMALL_P, SMALL_G, SMALL_PRIV);

    cr_assert_str_eq(MyPgp::ElGamal::decrypt(SMALL_HI, key).c_str(), "Hi", "Decryption of the known ciphertext failed");
}

Test(ElGamal_Decrypt, known_answer_block_mode)
{
    std::string key = makePublicKey(SMALL_P, SMALL_G, SMALL_PRIV);

    cr_assert_str_eq(MyPgp::ElGamal::decrypt(SMALL_HI, key, true).c_str(), "H", "Block mode must only decrypt one char");
}

Test(ElGamal_Decrypt, roundtrip_block_mode)
{
    std::string pubKey = makePublicKey(SMALL_P, SMALL_G, SMALL_PUB);
    std::string privKey = makePublicKey(SMALL_P, SMALL_G, SMALL_PRIV);

    std::string crypt = MyPgp::ElGamal::encrypt("Hello", pubKey + ":09", true);
    cr_assert_str_eq(MyPgp::ElGamal::decrypt(crypt, privKey, true).c_str(), "H");
}

Test(ElGamal_Decrypt, wrong_private_key_does_not_give_the_message)
{
    std::string pubKey = makePublicKey(SMALL_P, SMALL_G, SMALL_PUB);
    std::string wrongKey = makePublicKey(SMALL_P, SMALL_G, "0100");
    std::string crypt = MyPgp::ElGamal::encrypt("Hi", pubKey + ":0b");

    try {
        std::string res = MyPgp::ElGamal::decrypt(crypt, wrongKey);
        cr_assert_str_neq(res.c_str(), "Hi", "A wrong private key must not decrypt the message");
    } catch (MyPgp::MyPgpException &) {
    }
}

Test(ElGamal_Decrypt, wrong_number_of_key_parts)
{
    assertMyPgpError([] { MyPgp::ElGamal::decrypt(SMALL_HI, SMALL_P + "-" + SMALL_PRIV); }, "Wrong infos.");
    assertMyPgpError([] { MyPgp::ElGamal::decrypt(SMALL_HI, SMALL_P + "-" + SMALL_G + "-" + SMALL_PRIV + "-00"); }, "Wrong infos.");
}

Test(ElGamal_Decrypt, wrong_number_of_message_parts)
{
    std::string key = makePublicKey(SMALL_P, SMALL_G, SMALL_PRIV);

    assertMyPgpError([&] { MyPgp::ElGamal::decrypt(SMALL_C1, key); }, "Wrong infos.");
    assertMyPgpError([&] { MyPgp::ElGamal::decrypt("bd00-9101-fe00", key); }, "Wrong infos.");
}

Test(ElGamal_Decrypt, wrong_message_length)
{
    std::string key = makePublicKey(SMALL_P, SMALL_G, SMALL_PRIV);

    assertMyPgpError([&] { MyPgp::ElGamal::decrypt("bd00-9101fe", key); }, "Wrong message length.");
}

Test(ElGamal_Decrypt, decoded_value_over_255_fails)
{
    std::string key = makePublicKey(SMALL_P, SMALL_G, SMALL_PRIV);

    assertMyPgpError([&] { MyPgp::ElGamal::decrypt(SMALL_BAD, key); }, "Decryption failed.");
}

/* ------------------------------------------------------------------ */
/* checkKey                                                            */
/* ------------------------------------------------------------------ */

namespace {

    void assertNoThrow(const std::function<void()> &fn, const char *what)
    {
        try {
            fn();
        } catch (MyPgp::MyPgpException &e) {
            cr_assert_fail("%s: unexpected exception \"%s\"", what, e.what());
        }
    }
}

Test(ElGamal_CheckKey, generate_single_prime_is_valid)
{
    assertNoThrow([] { MyPgp::ElGamal::checkKey(SMALL_P, MyPgp::Mode::GENERATE); },
        "A prime without a seed must be accepted");
}

Test(ElGamal_CheckKey, generate_prime_with_seed_is_valid)
{
    assertNoThrow([] { MyPgp::ElGamal::checkKey(SMALL_P + ":01", MyPgp::Mode::GENERATE); },
        "A prime with a seed must be accepted");
}

Test(ElGamal_CheckKey, generate_too_many_seed_parts_is_rejected)
{
    assertMyPgpError([] { MyPgp::ElGamal::checkKey(SMALL_P + ":01:02", MyPgp::Mode::GENERATE); },
        "Invalid key format");
}

Test(ElGamal_CheckKey, generate_non_hex_prime_is_rejected)
{
    assertMyPgpError([] { MyPgp::ElGamal::checkKey("zz", MyPgp::Mode::GENERATE); },
        "Invalid key format");
}

Test(ElGamal_CheckKey, generate_empty_key_is_rejected)
{
    assertMyPgpError([] { MyPgp::ElGamal::checkKey("", MyPgp::Mode::GENERATE); },
        "Invalid key format");
}

Test(ElGamal_CheckKey, cipher_key_without_seed_is_valid)
{
    std::string key = makePublicKey(SMALL_P, SMALL_G, SMALL_PUB);

    assertNoThrow([&] { MyPgp::ElGamal::checkKey(key, MyPgp::Mode::CIPHER); },
        "A p-g-key triplet without a seed must be accepted");
}

Test(ElGamal_CheckKey, cipher_key_with_seed_is_valid)
{
    std::string key = makePublicKey(SMALL_P, SMALL_G, SMALL_PUB) + ":05";

    assertNoThrow([&] { MyPgp::ElGamal::checkKey(key, MyPgp::Mode::CIPHER); },
        "A p-g-key triplet with a seed must be accepted");
}

Test(ElGamal_CheckKey, cipher_wrong_number_of_dash_parts_is_rejected)
{
    assertMyPgpError([] { MyPgp::ElGamal::checkKey(SMALL_P + "-" + SMALL_G, MyPgp::Mode::CIPHER); },
        "Invalid key format");
    assertMyPgpError([] {
        MyPgp::ElGamal::checkKey(makePublicKey(SMALL_P, SMALL_G, SMALL_PUB) + "-00", MyPgp::Mode::CIPHER);
    }, "Invalid key format");
}

Test(ElGamal_CheckKey, cipher_too_many_seed_parts_is_rejected)
{
    std::string key = makePublicKey(SMALL_P, SMALL_G, SMALL_PUB) + ":01:02";

    assertMyPgpError([&] { MyPgp::ElGamal::checkKey(key, MyPgp::Mode::CIPHER); },
        "Invalid key format");
}

Test(ElGamal_CheckKey, cipher_non_hex_component_is_rejected)
{
    std::string key = makePublicKey("zz", SMALL_G, SMALL_PUB);

    assertMyPgpError([&] { MyPgp::ElGamal::checkKey(key, MyPgp::Mode::CIPHER); },
        "Invalid key format");
}

Test(ElGamal_CheckKey, cipher_non_hex_seed_is_rejected)
{
    std::string key = makePublicKey(SMALL_P, SMALL_G, SMALL_PUB) + ":zz";

    assertMyPgpError([&] { MyPgp::ElGamal::checkKey(key, MyPgp::Mode::CIPHER); },
        "Invalid key format");
}

Test(ElGamal_CheckKey, decipher_key_is_valid)
{
    std::string key = makePublicKey(SMALL_P, SMALL_G, SMALL_PRIV);

    assertNoThrow([&] { MyPgp::ElGamal::checkKey(key, MyPgp::Mode::DECIPHER); },
        "A p-g-key triplet must be accepted for deciphering");
}

Test(ElGamal_CheckKey, decipher_wrong_number_of_parts_is_rejected)
{
    assertMyPgpError([] { MyPgp::ElGamal::checkKey(SMALL_P + "-" + SMALL_PRIV, MyPgp::Mode::DECIPHER); },
        "Invalid key format");
}

Test(ElGamal_CheckKey, decipher_key_with_seed_is_rejected)
{
    std::string key = makePublicKey(SMALL_P, SMALL_G, SMALL_PRIV) + ":05";

    assertMyPgpError([&] { MyPgp::ElGamal::checkKey(key, MyPgp::Mode::DECIPHER); },
        "Invalid key format");
}

Test(ElGamal_CheckKey, decipher_non_hex_component_is_rejected)
{
    std::string key = makePublicKey(SMALL_P, SMALL_G, "zz");

    assertMyPgpError([&] { MyPgp::ElGamal::checkKey(key, MyPgp::Mode::DECIPHER); },
        "Invalid key format");
}