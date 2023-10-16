#include "gui/scrollbar.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace gui
{

void Scrollbar::updateViewPosition()
{
  if (m_offset.x < -0.5) m_offset.x = -0.5;
  if (m_offset.x >  0.5) m_offset.x = 0.5;
  if (m_offset.y < -0.5) m_offset.y = -0.5;
  if (m_offset.y >  0.5) m_offset.y = 0.5;
  const math::Vec scale = m_view->getViewScale();
  
  const double max_offset_x = scale.x/2;
  const double max_offset_y = scale.y/2;
  m_view->setViewPosition(math::Vec(
            m_offset.x * max_offset_x,
            m_offset.y * max_offset_y));
}

void Scrollbar::onClick(size_t button_id)
{
  if (button_id == m_buttonUp)    { m_offset.y -= 0.01; }
  if (button_id == m_buttonDown)  { m_offset.y += 0.01; }
  if (button_id == m_buttonLeft)  { m_offset.x -= 0.01; }
  if (button_id == m_buttonRight) { m_offset.x += 0.01; }

  updateViewPosition();
}


void Scrollbar::onHold(size_t button_id, double delta_time)
{
  if (button_id == m_buttonUp)    { m_offset.y -= 0.1 * delta_time; }
  if (button_id == m_buttonDown)  { m_offset.y += 0.1 * delta_time; }
  if (button_id == m_buttonLeft)  { m_offset.x -= 0.1 * delta_time; }
  if (button_id == m_buttonRight) { m_offset.x += 0.1 * delta_time; }

  updateViewPosition();
}

void Scrollbar::setValue(size_t slider_id, const math::Vec& val)
{
  if (slider_id == m_scrollHoriz)
    m_offset.x = val.x;
  if (slider_id == m_scrollVert)
    m_offset.y = val.y;

  updateViewPosition();
}

math::Vec Scrollbar::getValue(size_t slider_id)
{
  if (slider_id == m_scrollHoriz)
    return math::Vec(m_offset.x, 0);
  if (slider_id == m_scrollVert)
    return math::Vec(0, m_offset.y);

  return math::Vec();
}


} // namespace gui