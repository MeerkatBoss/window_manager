#include "gui/button.h"
#include "event/keys.h"
#include "gui/widget.h"
#include "math/transform.h"

namespace gui
{

bool Button::onMousePressed(event::MouseKey mouse_button)
{
  if (mouse_button != event::MouseKey::Left || !m_hovered)
    return false;

  m_controller.onClick(getId());
  m_pressed = true;

  return true;
}

bool Button::onMouseReleased(event::MouseKey mouse_button)
{
  if (mouse_button != event::MouseKey::Left || !m_pressed)
    return false;

  m_controller.onRelease(getId());
  m_pressed = false;

  return true;
}

bool Button::onMouseMoved(const math::Vec& position,
                          math::TransformStack& transform_stack)
{
  transform_stack.enterCoordSystem(transform());
  const math::Vec local_position = transform_stack.getCoordSystem()
                                    .restorePoint(position);
  transform_stack.exitCoordSystem();

  return m_hovered = (-0.5 < local_position.x && local_position.x < 0.5 &&
                      -0.5 < local_position.y && local_position.y < 0.5);
                      
}

bool Button::onUpdate(double delta_time)
{
  if (m_pressed)
  {
    m_controller.onHold(getId(), delta_time);
    return true;
  }
  return false;
}

void Button::draw(sf::RenderTarget& draw_target,
                  math::TransformStack& transform_stack)
{
  transform_stack.enterCoordSystem(transform());
  transform_stack.enterCoordSystem(m_textureTransform);

  const math::Transform& real_transform = transform_stack.getCoordSystem();

  sf::Sprite sprite(m_texture);
  sprite.setScale(real_transform.getScale());
  sprite.setPosition(real_transform.getPosition());

  draw_target.draw(sprite);

  transform_stack.exitCoordSystem();
  transform_stack.exitCoordSystem();
}

} // namespace ui
