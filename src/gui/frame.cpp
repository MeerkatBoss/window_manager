#include "gui/frame.h"
#include <cstdio>
#include "math/vec.h"

namespace gui
{

bool Frame::onMousePressed(event::MouseKey mouse_button)
{
  bool handled = Base::onMousePressed(mouse_button);
  if (handled)
  {
    return true;
  }

  if (mouse_button != event::MouseKey::Left || !isFocused())
    return false;

  m_moving = true;

  return true;
}

bool Frame::onMouseReleased(event::MouseKey mouse_button)
{
  bool handled = Base::onMouseReleased(mouse_button);

  if (mouse_button != event::MouseKey::Left || !m_moving)
    return handled;

  m_moving = false;

  return true;
}

bool Frame::onMouseMoved(const math::Vec& position,
                          math::TransformStack& transform_stack)
{
  bool handled = Base::onMouseMoved(position, transform_stack);
  
  transform_stack.enterCoordSystem(transform());
  const math::Vec local_position = transform_stack.getCoordSystem()
                                    .restorePoint(position);

  transform_stack.exitCoordSystem();

  const math::Vec parent_position = transform_stack.getCoordSystem()
                                    .restorePoint(position);
  if (m_moving)
  {
    transform().move(parent_position - m_lastPos);
    m_lastPos = parent_position;
    return true;
  }
  if (m_resizing)
  {
    math::Vec scale = 2*local_position;
    math::Vec old_scale = 2*transform().restorePoint(m_lastPos);

    const double min_scale = scale.x < scale.y ? scale.x : scale.y;
    const double min_old   = old_scale.x < old_scale.y
                              ? old_scale.x
                              : old_scale.y;

    math::Vec factor(
        min_scale / min_old,
        min_scale / min_old);

    transform().scale(factor);
    m_lastPos = parent_position;

    return true;
  }

  m_lastPos = parent_position;

  return isFocused() || handled;
}

void Frame::draw(sf::RenderTarget& draw_target,
                 math::TransformStack& transform_stack)
{
  transform_stack.enterCoordSystem(transform());

  const math::Transform& real_transform = transform_stack.getCoordSystem();

  sf::RectangleShape rect(real_transform.getScale());
  rect.setOrigin(real_transform.getScale() / 2);
  rect.setPosition(real_transform.getPosition());
  rect.setFillColor(sf::Color::Blue);

  draw_target.draw(rect);
  transform_stack.exitCoordSystem();

  Base::draw(draw_target, transform_stack);
}

} // namespace gui
