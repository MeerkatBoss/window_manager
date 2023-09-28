#include "gui/widget.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include "math/transform.h"

namespace gui
{

size_t Widget::s_idCounter = 0;

bool Widget::containsPoint(
                  const math::Point& point,
                  const math::Transform&  parent_transform)
{
  math::Transform realTransform = transform().withParent(parent_transform);
  math::Point localPoint = realTransform.restorePoint(point);

  return -1 < localPoint.x && localPoint.x < 1 &&
         -1 < localPoint.y && localPoint.y < 1;
}

} // namespace ui
