#include "gui/scrollbar.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace gui
{


void Scrollbar::draw(sf::RenderTarget& draw_target,
                  math::TransformStack& transform_stack)
{
  transform_stack.enterCoordSystem(transform());

  const math::Vec scale = m_view->getViewScale();
  
  const double max_offset_x = scale.x/2;
  const double max_offset_y = scale.y/2;
  m_view->setViewPosition(math::Vec(
            m_offset.x * max_offset_x,
            m_offset.y * max_offset_y));

  m_view->draw(draw_target, transform_stack);
  m_buttonUp.draw(draw_target, transform_stack);
  m_buttonDown.draw(draw_target, transform_stack);
  m_buttonLeft.draw(draw_target, transform_stack);
  m_buttonRight.draw(draw_target, transform_stack);
  m_horizontal.draw(draw_target, transform_stack);
  m_vertical.draw(draw_target, transform_stack);

  transform_stack.exitCoordSystem();
}

bool Scrollbar::onMouseMoved(const math::Vec& position,
                          math::TransformStack& transform_stack)
{
  bool handled = false;
  transform_stack.enterCoordSystem(transform());

  handled |= m_buttonUp.onMouseMoved(position, transform_stack);
  handled |= m_buttonDown.onMouseMoved(position, transform_stack);
  handled |= m_buttonLeft.onMouseMoved(position, transform_stack);
  handled |= m_buttonRight.onMouseMoved(position, transform_stack);
  handled |= m_vertical.onMouseMoved(position, transform_stack);
  handled |= m_horizontal.onMouseMoved(position, transform_stack);

  transform_stack.exitCoordSystem();

  return Base::onMouseMoved(position, transform_stack) | handled;
}

bool Scrollbar::onMouseReleased(event::MouseKey mouse_button)
{
  bool handled = false;
  handled |= m_buttonUp.onMouseReleased(mouse_button);
  handled |= m_buttonDown.onMouseReleased(mouse_button);
  handled |= m_buttonLeft.onMouseReleased(mouse_button);
  handled |= m_buttonRight.onMouseReleased(mouse_button);
  handled |= m_vertical.onMouseReleased(mouse_button);
  handled |= m_horizontal.onMouseReleased(mouse_button);

  return Base::onMouseReleased(mouse_button) | handled;
}

bool Scrollbar::onUpdate(double delta_time)
{
  bool handled = false;
  handled |= m_buttonUp.onUpdate(delta_time);
  handled |= m_buttonDown.onUpdate(delta_time);
  handled |= m_buttonLeft.onUpdate(delta_time);
  handled |= m_buttonRight.onUpdate(delta_time);
  handled |= m_vertical.onUpdate(delta_time);
  handled |= m_horizontal.onUpdate(delta_time);

  return Base::onUpdate(delta_time) | handled;
}

void Scrollbar::onClick(size_t button_id)
{
  if (button_id == m_buttonUp.getId())
  {
    m_offset.y -= 0.01;
    if (m_offset.y < -0.5)
      m_offset.y = -0.5;
  }
  if (button_id == m_buttonDown.getId())
  {
    m_offset.y += 0.01;
    if (m_offset.y > 0.5)
      m_offset.y = 0.5;
  }
  if (button_id == m_buttonLeft.getId())
  {
    m_offset.x -= 0.01;
    if (m_offset.x < -0.5)
      m_offset.x = -0.5;
  }
  if (button_id == m_buttonRight.getId())
  {
    m_offset.x += 0.01;
    if (m_offset.x > 0.5)
      m_offset.x = 0.5;
  }
}


void Scrollbar::onHold(size_t button_id, double delta_time)
{
  if (button_id == m_buttonUp.getId())
  {
    m_offset.y -= 0.1*delta_time;
    if (m_offset.y < -0.5)
      m_offset.y = -0.5;
  }
  if (button_id == m_buttonDown.getId())
  {
    m_offset.y += 0.1*delta_time;
    if (m_offset.y > 0.5)
      m_offset.y = 0.5;
  }
  if (button_id == m_buttonLeft.getId())
  {
    m_offset.x -= 0.1*delta_time;
    if (m_offset.x < -0.5)
      m_offset.x = -0.5;
  }
  if (button_id == m_buttonRight.getId())
  {
    m_offset.x += 0.1*delta_time;
    if (m_offset.x > 0.5)
      m_offset.x = 0.5;
  }
}

void Scrollbar::setValue(size_t slider_id, const math::Vec& val)
{
  if (slider_id == m_horizontal.getId())
    m_offset.x = val.x;
  if (slider_id == m_vertical.getId())
    m_offset.y = val.y;
}

math::Vec Scrollbar::getValue(size_t slider_id)
{
  if (slider_id == m_horizontal.getId())
    return math::Vec(m_offset.x, 0);
  else // if (slider_id == m_vertical.getId())
    return math::Vec(0, m_offset.y);
}


} // namespace gui
