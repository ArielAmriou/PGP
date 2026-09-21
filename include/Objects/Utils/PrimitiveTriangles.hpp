/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** ${descriptor}
*/

#ifndef PRIMITIVETRIANGLES_HPP
    #define PRIMITIVETRIANGLES_HPP
    #include <optional>
    #include "IObject.hpp"
    #include "Texture.hpp"

namespace My_pgp {
    class PrimitiveTriangle {
    public:
        PrimitiveTriangle(const Maths::Point3D &a, const Maths::Point3D &b,
            const Maths::Point3D &c, const Material &mat,
            std::optional<Texture> texture = std::nullopt);

        std::optional<HitInfo> fillHitInfo(Ray const &ray, Maths::Vector3D const &AB,
            Maths::Vector3D const &AC, double t, Maths::Vector2D uv) const;

        std::optional<HitInfo> rayTriangleMollerTrumboreAlgo(Ray const &ray,
            Maths::Vector3D AB, Maths::Vector3D AC, Maths::Vector3D h) const;

        std::optional<HitInfo> hits(Ray const &ray) const;

    private:

        static constexpr double EPSILON = 0.0000001;

        Maths::Point3D _a;
        Maths::Point3D _b;
        Maths::Point3D _c;
        Material _material;
        std::optional<Texture> _texture;

        static const Maths::Vector2D _uvA;
        static const Maths::Vector2D _uvB;
        static const Maths::Vector2D _uvC;
    };
}

#endif //PRIMITIVETRIANGLES_HPP