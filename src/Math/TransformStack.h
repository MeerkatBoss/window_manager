/**
 * @file transform_stack.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-10-03
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __MATH_TRANSFORM_STACK_H
#define __MATH_TRANSFORM_STACK_H

#include "Math/Transform.h"
#include "Util/DynArray.h"

namespace math
{

class TransformStack
{
public:
  TransformStack() : m_stack() { m_stack.pushBack(Transform()); }

  ~TransformStack() = default;

  void enterCoordSystem(const Transform& transform)
  {
    m_stack.pushBack(transform.withParent(m_stack.back()));
  }

  void exitCoordSystem()
  {
    if (m_stack.getSize() > 1)
    {
      m_stack.popBack();
    }
  }

  const Transform& getCoordSystem() const { return m_stack.back(); }

private:
  util::DynArray<Transform> m_stack;
};

} // namespace math

#endif /* TransformStack.h */
