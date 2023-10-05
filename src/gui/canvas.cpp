#include "gui/canvas.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "math/transform.h"
#include "math/transform_stack.h"
#include "math/vec.h"

namespace gui
{

void Canvas::applyBrush(const math::Vec& position)
{
  sf::CircleShape brush(m_penRadius);
  brush.setOrigin(m_penRadius / 2, m_penRadius / 2);
  brush.setPosition(position);
  brush.setFillColor(sf::Color::Black);
  m_renderTexture.draw(brush);
}

bool Canvas::onMousePressed(event::MouseKey mouse_button)
{
  if (mouse_button != event::MouseKey::Left || !m_hovered)
    return false;

  m_drawing = true;
  applyBrush(m_lastPos);

  return true;
}

bool Canvas::onMouseReleased(event::MouseKey mouse_button)
{
  if (mouse_button != event::MouseKey::Left || !m_drawing)
    return false;

  m_drawing = false;

  return true;
}

bool Canvas::onMouseMoved(const math::Vec& position,
                          math::TransformStack& transform_stack)
{
  transform_stack.enterCoordSystem(transform());
  transform_stack.enterCoordSystem(m_textureTransform);

  const math::Vec local_position = transform_stack.getCoordSystem()
                                    .restorePoint(position);
  transform_stack.exitCoordSystem();
  transform_stack.exitCoordSystem();

  double max_x = m_renderTexture.getSize().x;
  double max_y = m_renderTexture.getSize().y;

  m_hovered = (0 < local_position.x && local_position.x < max_x &&
               0 < local_position.y && local_position.y < max_y);

  if (m_hovered)
    m_lastPos = local_position;

  if (!m_drawing)
    return m_hovered;

  applyBrush(m_lastPos);

  return true;
}

void Canvas::draw(sf::RenderTarget& draw_target,
                  math::TransformStack& transform_stack)
{
  m_renderTexture.display();

  transform_stack.enterCoordSystem(transform());
  transform_stack.enterCoordSystem(m_textureTransform);
  const math::Transform& real_transform = transform_stack.getCoordSystem();

  sf::Sprite sprite(m_renderTexture.getTexture());
  sprite.setScale(real_transform.getScale());
  sprite.setPosition(real_transform.getPosition());
  draw_target.draw(sprite);

  transform_stack.exitCoordSystem();
  transform_stack.exitCoordSystem();

}


} // namespace gui
