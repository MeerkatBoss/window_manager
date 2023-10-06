#include "gui/frame.h"
#include <cstdio>

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

  m_captured = true;

  return true;
}

bool Frame::onMouseReleased(event::MouseKey mouse_button)
{
  Base::onMouseReleased(mouse_button);
  if (mouse_button != event::MouseKey::Left || !m_captured)
    return false;

  m_captured = false;

  return true;
}

bool Frame::onMouseMoved(const math::Vec& position,
                          math::TransformStack& transform_stack)
{
  Base::onMouseMoved(position, transform_stack);

  const math::Vec parent_position = transform_stack.getCoordSystem()
                                    .restorePoint(position);
  if (m_captured)
  {
    transform().move(parent_position - m_lastPos);
    m_lastPos = parent_position;
    return true;
  }

  m_lastPos = parent_position;

  return isFocused();;
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
  getDecorated()->draw(draw_target, transform_stack);

  transform_stack.exitCoordSystem();
}

} // namespace gui
