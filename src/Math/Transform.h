/**
 * @file transform.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief Offset, scale and rotation of scene object
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

#include "Math/Vec.h"

namespace math
{

class Transform
{
public:
  explicit Transform(const Point& offset = Point(0, 0),
                     const Vec&   scale  = Vec(1, 1)) :
      m_offset(offset), m_scale(scale)
  {
  }

  Transform(const Transform& other) = default;
  Transform& operator=(const Transform& other) = default;

  Point transformPoint(const Point& point) const
  {
    return Point(point.x * m_scale.x + m_offset.x,
                 point.y * m_scale.y + m_offset.y);
  }

  Point restorePoint(const Point& point) const
  {
    return Point((point.x - m_offset.x) / m_scale.x,
                 (point.y - m_offset.y) / m_scale.y);
  }

  Vec transformVector(const Vec& vector) const
  {
    return Vec(vector.x * m_scale.x, vector.y * m_scale.y);
  }

  Vec restoreVector(const Vec& vector) const
  {
    return Vec(vector.x / m_scale.x, vector.y / m_scale.y);
  }

  Transform withParent(const Transform& parent) const
  {
    const Vec offset = parent.m_offset + parent.transformVector(m_offset);
    const Vec scale  = parent.transformVector(m_scale);

    return Transform(offset, scale);
  }

  const Point& getOffset() const { return m_offset; }
  const Vec&   getScale() const { return m_scale; }

  void setOffset(const math::Point& offset) { m_offset = offset; }
  void setScale(const math::Vec& scale) { m_scale = scale; }

  void move(const Vec& delta) { m_offset += delta; }

  void scale(const Vec& scale)
  {
    m_scale.x *= scale.x;
    m_scale.y *= scale.y;
  }

private:
  Vec m_offset;
  Vec m_scale;
};

} // namespace math

#endif /* Transform.h */
