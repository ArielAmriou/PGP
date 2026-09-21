/*
** EPITECH PROJECT, 2026
** BS_My_pgp
** File description:
** DESCRIPTION
*/

#ifndef POINT3_HPP
    #define POINT3_HPP

    #include <cmath>

    #include "Vector.hpp"

namespace Maths {

    template<typename Type>
    using Point3 = Vector3<Type>;
    using Point3D = Point3<double>;
} 

#endif