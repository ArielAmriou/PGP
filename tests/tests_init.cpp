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

static void redirect_all_std()
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(Example, example_test)
{
    cr_assert(true);
}