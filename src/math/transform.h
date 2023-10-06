/**
 * @file transform.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief Position, scale and rotation of scene object
 *
 * @version 0.1
 * @date 2023-09-11
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __MATH_TRANSFORM_H
#define __MATH_TRANSFORM_H

#include <cmath>

#include <math.h>
#include "math/vec.h"

namespace math
{

class Transform
{
public:
  Transform(const Point&  position  = Point(0, 0),
            const Vec&    scale     = Vec(1, 1)) :
    m_position(position), m_scale(scale) {}

  Transform(const Transform& other) = default;
  Transform& operator=(const Transform& other) = default;

  Point transformPoint(const Point& point) const
  {
    return Point(
        point.x * m_scale.x + m_position.x,
        point.y * m_scale.y + m_position.y
        );
  }

  Point restorePoint(const Point& point) const
  {
    return Point(
        (point.x - m_position.x) / m_scale.x,
        (point.y - m_position.y) / m_scale.y
        );
  }

  Vec transformVector(const Vec& vector) const
  {
    return Vec(
        vector.x * m_scale.x,
        vector.y * m_scale.y
        );
  }

  Vec restoreVector(const Vec& vector) const
  {
    return Vec(
        vector.x / m_scale.x,
        vector.y / m_scale.y
        );
  }

  Transform withParent(const Transform& parent) const
  {
    const Vec position = parent.m_position + parent.transformVector(m_position);
    const Vec scale    = parent.transformVector(m_scale);

    return Transform(position, scale);
  }

  const Point& getPosition() const { return m_position; }
  const Vec&   getScale()    const { return m_scale; }

  void setPosition(const math::Point& position) { m_position = position; }
  void setScale   (const math::Vec&   scale)    { m_scale    = scale;    }

  void move(const Vec& delta) { m_position += delta; }

  void scale(const Vec& scale)
  {
    m_scale.x *= scale.x;
    m_scale.y *= scale.y;
  }

private:
  Vec m_position;
  Vec m_scale;
};

} // namespace math

#endif /* transform.h */
