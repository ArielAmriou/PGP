/*
** EPITECH PROJECT, 2026
** My_pgp
** File description:
** ${descriptor}
*/

#ifndef CAMERA_HPP
    #define CAMERA_HPP

#include "Quaternion.hpp"
#include "Vector.hpp"
#include "Rectangle.hpp"
#include "Ray.hpp"

namespace My_pgp {
    class Camera {
        public:
            Camera() = default;
            Camera(Maths::Vector2U const &resolution, Maths::Point3D const
                &position, Maths::Quaternion const &rotation, double fov);

            Ray ray(const Maths::Vector2D &v) const noexcept;
            void updateCamera() noexcept;

            void setResolution(Maths::Vector2U const &reso) noexcept;
            void setPosition(Maths::Point3D const &pos) noexcept;
            void setRotation(Maths::Quaternion const &rot) noexcept;
            void setFov(double fov) noexcept;

            [[nodiscard]] double getFov() const noexcept;
            [[nodiscard]] std::size_t getNbPixel() const noexcept;
            [[nodiscard]] Maths::Vector2U getResolution() const noexcept;
            [[nodiscard]] Maths::Point3D getPosition() const noexcept;
            [[nodiscard]] Maths::Quaternion getRotation() const noexcept;
            [[nodiscard]] Maths::Vector3D forward() const noexcept;
            [[nodiscard]] Maths::Vector3D right() const noexcept;
            [[nodiscard]] Maths::Vector3D up() const noexcept;
        private:
            Maths::Vector2U _resolution;
            std::size_t _nbPix;
            Maths::Point3D _position;
            Maths::Quaternion _rotation;
            double _fov;
            Rectangle _screen;
    };
}

#endif //CAMERA_HPP