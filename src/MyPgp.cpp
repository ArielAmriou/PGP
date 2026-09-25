/*
** EPITECH PROJECT, 2026
** MyPgp
** File description:
** MyPgp
*/

#include "MyPgp.hpp"
#include "Exception.hpp"

#include "Xor.hpp"
#include <iostream>
#include <bits/stdc++.h>

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