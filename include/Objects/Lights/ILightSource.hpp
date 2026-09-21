/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** ${descriptor}
*/

#ifndef ILIGHTSOURCE_HPP
    #define ILIGHTSOURCE_HPP
    #include "Color.hpp"
    #include "Ray.hpp"
    #include "Vector.hpp"

namespace My_pgp {
    class ILightSource {
    public:
        virtual ~ILightSource() = default;
        [[nodiscard]] virtual Maths::Point3D getPosition() const = 0;

        [[nodiscard]] virtual Maths::Color getLightAmount(const Ray &) const = 0;
    };
}

#endif //ILIGHTSOURCE_HPP