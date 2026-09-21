/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** Rectangle
*/

#ifndef RECTANGLE_HPP_
    #define RECTANGLE_HPP_

#include "Point.hpp"
#include "Vector.hpp"

namespace My_pgp {
    class Rectangle {
        public:
            Rectangle() = default;
            Rectangle(const Maths::Point3D &origin,
                const Maths::Vector3D &topSide,
                const Maths::Vector3D &leftSide):
            _origin(origin),
            _topSide(topSide),
            _leftSide(leftSide) {}

            Maths::Point3D pointAt(double u, double v) const noexcept;
            void setOrigin(const Maths::Point3D &origin) noexcept
                { _origin = origin; };
            void setTopSide(const Maths::Vector3D &topSide) noexcept
                { _topSide = topSide; };
            void setLeftSide(const Maths::Vector3D &leftSide) noexcept
                { _leftSide = leftSide; };
        private:
            Maths::Point3D _origin;
            Maths::Vector3D _topSide;
            Maths::Vector3D _leftSide;
    };
}

#endif /* !RECTANGLE_HPP_ */
