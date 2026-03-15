#pragma once
#include <cmath>

namespace Refaf::Math
{
    template <typename T>
    struct Vector2
    {
        T x, y;

        constexpr Vector2() : x(0), y(0) {}
        constexpr Vector2(T x, T y) : x(x), y(y) {}

        constexpr Vector2 operator+(const Vector2 &other) const
        {
            return Vector2(x + other.x, y + other.y);
        }

        constexpr Vector2 operator-(const Vector2 &other) const
        {
            return Vector2(x - other.x, y - other.y);
        }

        constexpr Vector2 operator*(T scalar) const
        {
            return Vector2(x * scalar, y * scalar);
        }

        constexpr Vector2 operator/(T scalar) const
        {
            return Vector2(x / scalar, y / scalar);
        }

        constexpr Vector2 &operator+=(const Vector2 &other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        constexpr Vector2 &operator-=(const Vector2 &other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        constexpr bool operator==(const Vector2 &other) const
        {
            return x == other.x && y == other.y;
        }

        auto Magnitude() const
        {
            return std::sqrt(x * x + y * y);
        }

        constexpr T MagnitudeSquared() const
        {
            return x * x + y * y;
        }

        Vector2 Normalized() const
        {
            auto len = Magnitude();
            if (len > 0)
            {
                return Vector2(x / len, y / len);
            }
            return Vector2(0, 0);
        }

        constexpr T Dot(const Vector2 &other) const
        {
            return x * other.x + y * other.y;
        }
    };

    template <typename T>
    constexpr Vector2<T> operator*(T scalar, const Vector2<T> &v)
    {
        return v * scalar;
    }

    using Vector2f = Vector2<float>;

    static_assert(sizeof(Vector2f) == 8);

} // namespace Refaf::Math
