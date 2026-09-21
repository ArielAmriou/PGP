/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** DESCRIPTION
*/

#ifndef ALIGHTSOURCE_HPP
    #define ALIGHTSOURCE_HPP
    #include "ILightSource.hpp"

namespace My_pgp {
    class ALightSource : public ILightSource {
    public:
        explicit ALightSource(const Maths::Point3D &pos);

        [[nodiscard]] Maths::Point3D getPosition() const override;
    protected:
        Maths::Point3D _pos;
    };
} // My_pgp

#endif
