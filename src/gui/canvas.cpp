#include "gui/canvas.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "event/keys.h"
#include "gui/widget.h"
#include "math/transform.h"
#include "math/transform_stack.h"
#include "math/vec.h"
#include "tool/tool.h"
#include "tool/tool_palette.h"

namespace gui
{

bool Canvas::onMousePressed(event::MouseKey mouse_button)
{
  if (mouse_button == event::MouseKey::Right)
  {
    m_palette.getActiveTool()->onSecondaryButton(
        tool::ButtonState::Pressed, m_lastPos, *this);
  }

  if (mouse_button != event::MouseKey::Left || !m_hovered)
    return false;

  m_palette.getActiveTool()->onMainButton(
      tool::ButtonState::Pressed, m_lastPos, *this);

  return true;
}

bool Canvas::onMouseReleased(event::MouseKey mouse_button)
{
  if (mouse_button == event::MouseKey::Right)
  {
    m_palette.getActiveTool()->onSecondaryButton(
        tool::ButtonState::Released, m_lastPos, *this);
  }

  if (mouse_button != event::MouseKey::Left)
    return false;

  m_palette.getActiveTool()->onMainButton(
      tool::ButtonState::Released, m_lastPos, *this);

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

  m_palette.getActiveTool()->onMove(m_lastPos, *this);

  return true;
}

bool Canvas::onKeyboardPressed(event::KeyboardKey key)
{
  using event::KeyboardKey;

  if (key == KeyboardKey::LShift || key == KeyboardKey::RShift)
  {
    m_palette.getActiveTool()->onModifier1(
        tool::ButtonState::Pressed, m_lastPos, *this);
    return true;
  }

  if (key == KeyboardKey::LControl || key == KeyboardKey::RControl)
  {
    m_palette.getActiveTool()->onModifier2(
        tool::ButtonState::Pressed, m_lastPos, *this);
    return true;
  }

  if (key == KeyboardKey::LAlt || key == KeyboardKey::RAlt)
  {
    m_palette.getActiveTool()->onModifier3(
        tool::ButtonState::Pressed, m_lastPos, *this);
    return true;
  }

  if (key == KeyboardKey::Escape)
  {
    m_palette.getActiveTool()->onCancel(m_lastPos, *this);
    return true;
  }

  if (key == KeyboardKey::Enter)
  {
    m_palette.getActiveTool()->onConfirm(m_lastPos, *this);
    return true;
  }

  return false;
}

bool Canvas::onKeyboardReleased(event::KeyboardKey key)
{
  using event::KeyboardKey;

  if (key == KeyboardKey::LShift || key == KeyboardKey::RShift)
  {
    m_palette.getActiveTool()->onModifier1(
        tool::ButtonState::Released, m_lastPos, *this);
    return true;
  }

  if (key == KeyboardKey::LControl || key == KeyboardKey::RControl)
  {
    m_palette.getActiveTool()->onModifier2(
        tool::ButtonState::Released, m_lastPos, *this);
    return true;
  }

  if (key == KeyboardKey::LAlt || key == KeyboardKey::RAlt)
  {
    m_palette.getActiveTool()->onModifier3(
        tool::ButtonState::Released, m_lastPos, *this);
    return true;
  }

  return false;
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

  Widget* tool_widget = m_palette.getActiveTool()->getWidget();

  if (tool_widget)
    tool_widget->draw(draw_target, transform_stack);

  transform_stack.exitCoordSystem();
  transform_stack.exitCoordSystem();
}


} // namespace gui