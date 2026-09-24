/*
** EPITECH PROJECT, 2026
** MyPgp
** File description:
** Main
*/

#include "MyPgp.hpp"
#include "Exception.hpp"
#include <iostream>

int main(int ac, char **av)
{
    MyPgp::MyPgp pgp;

    try {
        pgp.run();
    } catch (MyPgp::MyPgpException &e) {
        std::cerr << e.what() << std::endl;
        return MyPgp::EPIERROR;
    }
    return MyPgp::EPISUCCESS;
}
