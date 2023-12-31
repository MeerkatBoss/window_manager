#include "GUI/Widget.h"

#include "Math/Transform.h"
#include "Math/Vec.h"

namespace gui
{

size_t Widget::s_idCounter = 0;

math::Transform Widget::getLocalTransform(void) const
{
  return math::Transform(getPosition());
}

bool Widget::containsPoint(const math::Point&    point,
                           math::TransformStack& transform_stack) const
{
  transform_stack.enterCoordSystem(getLocalTransform());

  const math::Transform& transform = transform_stack.getCoordSystem();

  const math::Vec   local_size  = getSize();
  const math::Point local_point = transform.restorePoint(point);

  const bool contains = 0 <= local_point.x && local_point.x <= local_size.x &&
                        0 <= local_point.y && local_point.y <= local_size.y;

  transform_stack.exitCoordSystem();

  return contains;
}

void Widget::onLayoutUpdate(const layout::LayoutBox& parent_box)
{
  m_layoutBox->updateParent(parent_box);
}

} // namespace gui
