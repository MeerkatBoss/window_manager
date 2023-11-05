/**
 * @file vec.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief 3D vector with double-precision floating-point coordinates
 *
 * @version 0.1
 * @date 2023-09-10
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __MATH_VEC_H
#define __MATH_VEC_H

#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace math
{

struct Vec
{
  double x, y;

  static const Vec UnitX;
  static const Vec UnitY;

  Vec(double x_coord, double y_coord) : x(x_coord), y(y_coord) {}

  Vec() : x(0), y(0) {}

  Vec(const Vec& other) = default;
  Vec& operator=(const Vec& other) = default;

  Vec(const sf::Vector2f& sf_vector) : x(sf_vector.x), y(sf_vector.y) {}
  operator sf::Vector2f() const { return sf::Vector2f(x, y); }

  ~Vec()
  {
    x = NAN;
    y = NAN;
  }

  Vec& operator+=(const Vec& other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  Vec& operator-=(const Vec& other)
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Vec& operator*=(double scale)
  {
    x *= scale;
    y *= scale;
    return *this;
  }

  Vec& operator/=(double scale) { return *this *= 1.0 / scale; }

  Vec operator+(const Vec& other) const { return Vec(*this) += other; }
  Vec operator-(const Vec& other) const { return Vec(*this) -= other; }

  Vec operator*(double scale) const { return Vec(*this) *= scale; }
  Vec operator/(double scale) const { return Vec(*this) /= scale; }

  Vec operator-() const { return *this * (-1); }
  Vec operator+() const { return *this; }

  double length() const { return hypot(x, y); }

  bool isZero() const { return fabs(length()) < EPS; }

  Vec normalized() const { return *this / length(); }

  Vec projectOn(const Vec& other) const
  {
    if (other.isZero())
    {
      return Vec(NAN, NAN);
    }
    const double scale = dotProduct(*this, other) / dotProduct(other, other);
    return other * scale;
  }

  bool isParallelTo(const Vec& other) const
  {
    return fabs(Vec::crossProduct(*this, other)) < 0;
  }

  static double dotProduct(const Vec& vec1, const Vec& vec2)
  {
    return vec1.x * vec2.x + vec1.y * vec2.y;
  }

  static double crossProduct(const Vec& vec1, const Vec& vec2)
  {
    return vec1.x * vec2.y - vec1.y * vec2.x;
  }

private:
  static constexpr double EPS = 1e-6;
};

inline Vec  operator*(double scale, const Vec& vec) { return vec * scale; }
inline bool operator==(const Vec& lhs, const Vec& rhs)
{
  return (lhs - rhs).isZero();
}
inline bool operator!=(const Vec& lhs, const Vec& rhs) { return !(lhs == rhs); }

using Point = Vec;

} // namespace math

#endif /* Vec.h */
