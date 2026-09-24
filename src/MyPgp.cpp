/*
** EPITECH PROJECT, 2026
** MyPgp
** File description:
** MyPgp
*/

#include "MyPgp.hpp"
#include "Exception.hpp"

namespace MyPgp {

    void MyPgp::run()
    {
        try {
            return;
        } catch (MyPgpException &e) {
            throw e;
        }
    }

}