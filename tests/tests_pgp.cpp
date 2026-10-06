/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** Unit tests for pgp-aes and pgp-xor
*/

#include <criterion/criterion.h>

#include <cstring>
#include <string>
#include <utility>

#include "AES.hpp"
#include "Xor.hpp"
#include "RSA.hpp"
#include "Exception.hpp"
#include "MyPgp.hpp"

namespace {

    using CipherFn = MyPgp::ACipher::CipherFn;

    const std::string SYM_KEY = "57696e74657220697320636f6d696e67";
    const std::string MSG16 = "All men must die";

    const std::string VECTOR_PUB =
        "010001-c9f91a9ff3bd6d84005b9cc8448296330bd23480f8cf8b36fd4edd0a8cd925de139a0076b962f4d57f50d6f9e64e7c41587784488f923dd60136c763fd602fb3";

    const std::string VECTOR_RSA_LINE =
        "97f2af4c1b712008c1e46935f446756443a8a700f20581d138e4e6916afe5c5f"
        "9b9d6eaa0a870374b686f1a024f9bbb88c23c766654579339caf55afd149d41d";

    const std::string AES_BLOCK_LINE = "744ce22c385958348f0df26eceb62eef";

    const std::string OWN_N =
        "971d2f524e3652313e3863b8c528d7eebdf1aef8a11ff0e0424af623a37c0429"
        "f700b614e537d52a01e83b98532630baf41e974c68431083528b8a59b1e1fed1";

    const std::string OWN_D =
        "0149051263695d84f216d526eb1641f2b1b4603483dfeb5bb88c2717fd1bd679"
        "3bda93f83d8d14005e43d125e7a8fc64195c339b5ab4bf3d884c658ff969197e";

    const std::string OWN_D_WRONG =
        "0349051263695d84f216d526eb1641f2b1b4603483dfeb5bb88c2717fd1bd679"
        "3bda93f83d8d14005e43d125e7a8fc64195c339b5ab4bf3d884c658ff969197e";

    const std::string OWN_PUB = "010001-" + OWN_N;
    const std::string OWN_PRIV = OWN_D + "-" + OWN_N;
    const std::string OWN_PRIV_WRONG = OWN_D_WRONG + "-" + OWN_N;

    const std::string OWN_RSA_LINE =
        "4953c87bd0dce3351a754e4e884900df9f9bcbef07b28499c4a365656ce79617"
        "9e137057b4302809bd81ab0ad8c8031331b611b767c92321673e2a7522eedf2f";


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

    void expectEq(const std::string &actual, const std::string &expected, const char *what)
    {
        cr_assert(actual == expected, "%s\n  expected: %s\n  actual:   %s",
            what, expected.c_str(), actual.c_str());
    }

    void expectNe(const std::string &actual, const std::string &unexpected, const char *what)
    {
        cr_assert(actual != unexpected, "%s\n  value: %s", what, actual.c_str());
    }

    std::pair<std::string, std::string> splitOutput(const std::string &out)
    {
        auto pos = out.find('\n');

        cr_assert(pos != std::string::npos, "Output must be \"<rsa line>\\n<message line>\", got %s", out.c_str());
        cr_assert_eq(out.find('\n', pos + 1), std::string::npos, "Only one '\\n' expected in the output");
        return {out.substr(0, pos), out.substr(pos + 1)};
    }

    template <CipherFn ENC>
    void checkComposition(const std::string &msg, const std::string &symKey, bool block,
        const std::string &pub, const std::string &expectedRsaLine)
    {
        std::string out = MyPgp::MyPgp::encrypt<ENC>(msg, symKey + ":" + pub, block);
        auto [line1, line2] = splitOutput(out);

        std::string expectedMsgLine = ENC(msg, symKey, block);

        expectEq(line1, expectedRsaLine, "RSA line (encrypted symmetric key) is wrong");
        expectEq(line2, expectedMsgLine, "Message line must be the plain symmetric encryption of the message");
    }

    template <CipherFn ENC, CipherFn DEC>
    void checkRoundtrip(const std::string &msg, const std::string &symKey, bool block)
    {
        std::string out = MyPgp::MyPgp::encrypt<ENC>(msg, symKey + ":" + OWN_PUB, block);
        auto [line1, line2] = splitOutput(out);
        std::string res = MyPgp::MyPgp::decrypt<DEC>(line2, line1 + ":" + OWN_PRIV, block);

        expectEq(res, msg, "Decrypt(Encrypt(msg)) must give back msg");
    }

    template <CipherFn ENC, CipherFn DEC>
    void checkDecryptWithGivenRsaLine(const std::string &msg, bool block)
    {
        std::string symCrypt = ENC(msg, SYM_KEY, block);
        std::string res = MyPgp::MyPgp::decrypt<DEC>(symCrypt, OWN_RSA_LINE + ":" + OWN_PRIV, block);

        expectEq(res, msg, "Decryption with a known RSA line failed");
    }

    template <CipherFn ENC, CipherFn DEC>
    void checkWrongPrivateKey(const std::string &msg, bool block)
    {
        std::string out = MyPgp::MyPgp::encrypt<ENC>(msg, SYM_KEY + ":" + OWN_PUB, block);
        auto [line1, line2] = splitOutput(out);
        std::string res;
        bool threw = false;

        try {
            res = MyPgp::MyPgp::decrypt<DEC>(line2, line1 + ":" + OWN_PRIV_WRONG, block);
        } catch (...) {
            threw = true;
        }
        if (!threw)
            expectNe(res, msg, "A wrong private key must not decrypt the message");
    }

    template <CipherFn ENC, CipherFn DEC>
    void checkWrongArgs()
    {
        assertMyPgpError([] { MyPgp::MyPgp::encrypt<ENC>("Hi", SYM_KEY); }, "Wrong args.");
        assertMyPgpError([] { MyPgp::MyPgp::encrypt<ENC>("Hi", SYM_KEY + ":" + OWN_PUB + ":00"); }, "Wrong args.");
        assertMyPgpError([] { MyPgp::MyPgp::decrypt<DEC>("00", OWN_RSA_LINE); }, "Wrong args.");
        assertMyPgpError([] { MyPgp::MyPgp::decrypt<DEC>("00", OWN_RSA_LINE + ":" + OWN_PRIV + ":00"); }, "Wrong args.");
    }
}

Test(PgpAes_Encrypt, subject_vector_block_mode)
{
    std::string out = MyPgp::MyPgp::encrypt<MyPgp::AES::encrypt>(MSG16, SYM_KEY + ":" + VECTOR_PUB, true);

    std::string expected = VECTOR_RSA_LINE + "\n" + AES_BLOCK_LINE;

    expectEq(out, expected, "Output differs from the subject example");
}

Test(PgpAes_Encrypt, rsa_line_with_subject_key)
{
    checkComposition<MyPgp::AES::encrypt>(MSG16, SYM_KEY, true, VECTOR_PUB, VECTOR_RSA_LINE);
}

Test(PgpAes_Encrypt, rsa_line_with_own_key)
{
    checkComposition<MyPgp::AES::encrypt>(MSG16, SYM_KEY, true, OWN_PUB, OWN_RSA_LINE);
}

Test(PgpAes_Encrypt, rsa_line_does_not_depend_on_block_mode)
{
    checkComposition<MyPgp::AES::encrypt>(MSG16, SYM_KEY, false, VECTOR_PUB, VECTOR_RSA_LINE);
}

Test(PgpAes_Encrypt, rsa_line_has_modulus_size)
{
    std::string out = MyPgp::MyPgp::encrypt<MyPgp::AES::encrypt>(MSG16, SYM_KEY + ":" + VECTOR_PUB, true);
    auto [line1, line2] = splitOutput(out);

    cr_assert_eq(line1.size(), 128UL, "A 512 bit modulus gives 64 bytes, so 128 hex chars");
    cr_assert_eq(line1.find_first_not_of("0123456789abcdef"), std::string::npos, "RSA line must be lowercase hex");
}

Test(PgpAes_Encrypt, different_public_key_changes_only_the_rsa_line)
{
    std::string a = MyPgp::MyPgp::encrypt<MyPgp::AES::encrypt>(MSG16, SYM_KEY + ":" + VECTOR_PUB, true);
    std::string b = MyPgp::MyPgp::encrypt<MyPgp::AES::encrypt>(MSG16, SYM_KEY + ":" + OWN_PUB, true);
    auto [a1, a2] = splitOutput(a);
    auto [b1, b2] = splitOutput(b);

    expectNe(a1, b1, "Different public keys must give different RSA lines");
    expectEq(a2, b2, "The message line only depends on the symmetric key");
}

Test(PgpAes_Encrypt, different_symmetric_key_changes_both_lines)
{
    std::string other = "000102030405060708090a0b0c0d0e0f";
    std::string a = MyPgp::MyPgp::encrypt<MyPgp::AES::encrypt>(MSG16, SYM_KEY + ":" + OWN_PUB, true);
    std::string b = MyPgp::MyPgp::encrypt<MyPgp::AES::encrypt>(MSG16, other + ":" + OWN_PUB, true);
    auto [a1, a2] = splitOutput(a);
    auto [b1, b2] = splitOutput(b);

    expectNe(a1, b1, "RSA lines must differ for different symmetric keys");
    expectNe(a2, b2, "Message lines must differ for different symmetric keys");
}

Test(PgpAes_Encrypt, is_deterministic)
{
    std::string key = SYM_KEY + ":" + OWN_PUB;

    std::string first = MyPgp::MyPgp::encrypt<MyPgp::AES::encrypt>(MSG16, key, true);
    std::string second = MyPgp::MyPgp::encrypt<MyPgp::AES::encrypt>(MSG16, key, true);

    expectEq(second, first, "Same input must give the same output");
}

Test(PgpAes_Encrypt, wrong_number_of_key_parts)
{
    checkWrongArgs<MyPgp::AES::encrypt, MyPgp::AES::decrypt>();
}

Test(PgpAes_Decrypt, known_rsa_line_block_mode)
{
    std::string res = MyPgp::MyPgp::decrypt<MyPgp::AES::decrypt>(AES_BLOCK_LINE, OWN_RSA_LINE + ":" + OWN_PRIV, true);

    expectEq(res, MSG16, "Known ciphertext must decrypt to the original message");
}

Test(PgpAes_Decrypt, known_rsa_line_non_block_mode)
{
    checkDecryptWithGivenRsaLine<MyPgp::AES::encrypt, MyPgp::AES::decrypt>(MSG16, false);
}

Test(PgpAes_Decrypt, roundtrip_block_mode)
{
    checkRoundtrip<MyPgp::AES::encrypt, MyPgp::AES::decrypt>(MSG16, SYM_KEY, true);
}

Test(PgpAes_Decrypt, roundtrip_short_message)
{
    checkRoundtrip<MyPgp::AES::encrypt, MyPgp::AES::decrypt>("Hi", SYM_KEY, false);
}

Test(PgpAes_Decrypt, roundtrip_exact_block_size)
{
    checkRoundtrip<MyPgp::AES::encrypt, MyPgp::AES::decrypt>(MSG16, SYM_KEY, false);
}

Test(PgpAes_Decrypt, roundtrip_long_message)
{
    checkRoundtrip<MyPgp::AES::encrypt, MyPgp::AES::decrypt>(
        "Winter is coming. All men must die. A Lannister always pays his debts.", SYM_KEY, false);
}

Test(PgpAes_Decrypt, roundtrip_key_with_leading_zero_byte)
{
    checkRoundtrip<MyPgp::AES::encrypt, MyPgp::AES::decrypt>(
        MSG16, "00112233445566778899aabbccddeeff", true);
}

Test(PgpAes_Decrypt, roundtrip_key_with_trailing_zero_byte, .disabled = true)
{
    checkRoundtrip<MyPgp::AES::encrypt, MyPgp::AES::decrypt>(
        MSG16, "57696e74657220697320636f6d696e00", true);
}

Test(PgpAes_Decrypt, wrong_private_key_does_not_give_the_message)
{
    checkWrongPrivateKey<MyPgp::AES::encrypt, MyPgp::AES::decrypt>(MSG16, true);
}

Test(PgpXor_Encrypt, rsa_line_with_subject_key)
{
    checkComposition<MyPgp::Xor::encrypt>(MSG16, SYM_KEY, true, VECTOR_PUB, VECTOR_RSA_LINE);
}

Test(PgpXor_Encrypt, rsa_line_with_own_key)
{
    checkComposition<MyPgp::Xor::encrypt>(MSG16, SYM_KEY, true, OWN_PUB, OWN_RSA_LINE);
}

Test(PgpXor_Encrypt, rsa_line_does_not_depend_on_block_mode)
{
    checkComposition<MyPgp::Xor::encrypt>(MSG16, SYM_KEY, false, VECTOR_PUB, VECTOR_RSA_LINE);
}

Test(PgpXor_Encrypt, rsa_line_is_the_same_as_pgp_aes)
{
    std::string xorOut = MyPgp::MyPgp::encrypt<MyPgp::Xor::encrypt>(MSG16, SYM_KEY + ":" + VECTOR_PUB, true);
    std::string aesOut = MyPgp::MyPgp::encrypt<MyPgp::AES::encrypt>(MSG16, SYM_KEY + ":" + VECTOR_PUB, true);

    std::string xorLine = splitOutput(xorOut).first;
    std::string aesLine = splitOutput(aesOut).first;

    expectEq(xorLine, aesLine, "The RSA step must not depend on the symmetric cipher");
}

Test(PgpXor_Encrypt, different_public_key_changes_only_the_rsa_line)
{
    std::string a = MyPgp::MyPgp::encrypt<MyPgp::Xor::encrypt>(MSG16, SYM_KEY + ":" + VECTOR_PUB, true);
    std::string b = MyPgp::MyPgp::encrypt<MyPgp::Xor::encrypt>(MSG16, SYM_KEY + ":" + OWN_PUB, true);
    auto [a1, a2] = splitOutput(a);
    auto [b1, b2] = splitOutput(b);

    expectNe(a1, b1, "Different public keys must give different RSA lines");
    expectEq(a2, b2, "The message line only depends on the symmetric key");
}

Test(PgpXor_Encrypt, is_deterministic)
{
    std::string key = SYM_KEY + ":" + OWN_PUB;

    std::string first = MyPgp::MyPgp::encrypt<MyPgp::Xor::encrypt>(MSG16, key, true);
    std::string second = MyPgp::MyPgp::encrypt<MyPgp::Xor::encrypt>(MSG16, key, true);

    expectEq(second, first, "Same input must give the same output");
}

Test(PgpXor_Encrypt, wrong_number_of_key_parts)
{
    checkWrongArgs<MyPgp::Xor::encrypt, MyPgp::Xor::decrypt>();
}

Test(PgpXor_Decrypt, known_rsa_line_block_mode)
{
    checkDecryptWithGivenRsaLine<MyPgp::Xor::encrypt, MyPgp::Xor::decrypt>(MSG16, true);
}

Test(PgpXor_Decrypt, known_rsa_line_non_block_mode)
{
    checkDecryptWithGivenRsaLine<MyPgp::Xor::encrypt, MyPgp::Xor::decrypt>(MSG16, false);
}

Test(PgpXor_Decrypt, roundtrip_block_mode)
{
    checkRoundtrip<MyPgp::Xor::encrypt, MyPgp::Xor::decrypt>(MSG16, SYM_KEY, true);
}

Test(PgpXor_Decrypt, roundtrip_exact_key_size)
{
    checkRoundtrip<MyPgp::Xor::encrypt, MyPgp::Xor::decrypt>(MSG16, SYM_KEY, false);
}

Test(PgpXor_Decrypt, roundtrip_key_with_leading_zero_byte)
{
    checkRoundtrip<MyPgp::Xor::encrypt, MyPgp::Xor::decrypt>(
        MSG16, "00112233445566778899aabbccddeeff", true);
}

Test(PgpXor_Decrypt, roundtrip_key_with_trailing_zero_byte, .disabled = true)
{
    checkRoundtrip<MyPgp::Xor::encrypt, MyPgp::Xor::decrypt>(
        MSG16, "57696e74657220697320636f6d696e00", true);
}

Test(PgpXor_Decrypt, wrong_private_key_does_not_give_the_message)
{
    checkWrongPrivateKey<MyPgp::Xor::encrypt, MyPgp::Xor::decrypt>(MSG16, true);
}
