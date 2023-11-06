#include "GUI/Button.h"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

#include "Event/Keys.h"
#include "GUI/Layout/LayoutBox.h"
#include "GUI/Widget.h"
#include "Math/Transform.h"
#include "Math/TransformStack.h"
#include "Math/Vec.h"

namespace gui
{

bool Button::onMousePressed(const math::Vec& position,
                            event::MouseKey mouse_button,
                            math::TransformStack& transform_stack)
{
  onMouseMoved(position, transform_stack);

  if (mouse_button != event::MouseKey::Left || !m_hovered)
    return false;

  m_controller.onClick(getId());
  m_pressed = true;

  return true;
}

bool Button::onMouseReleased(const math::Vec& position,
                            event::MouseKey mouse_button,
                            math::TransformStack& transform_stack)
{
  onMouseMoved(position, transform_stack);

  if (mouse_button != event::MouseKey::Left || !m_pressed)
    return false;

  m_controller.onRelease(getId());
  m_pressed = false;

  return true;
}

bool Button::onMouseMoved(const math::Vec&      position,
                          math::TransformStack& transform_stack)
{
  return m_hovered = containsPoint(position, transform_stack);
}

bool Button::onTick(double delta_time)
{
  if (m_pressed)
  {
    m_controller.onHold(getId(), delta_time);
    return true;
  }
  return false;
}

void Button::draw(sf::RenderTarget&     draw_target,
                  math::TransformStack& transform_stack)
{
  const auto [tl, tr, bl, br] = layout::getRect(getSize());

  const math::Vec tex_size(m_texture.getSize().x, m_texture.getSize().y);
  const auto [tex_tl, tex_tr, tex_bl, tex_br] = layout::getRect(tex_size);

  transform_stack.enterCoordSystem(getLocalTransform());
  const math::Transform& real_transform = transform_stack.getCoordSystem();

  sf::VertexArray array(sf::TriangleStrip, 4);
  array[0] = sf::Vertex(real_transform.transformPoint(tl), tex_tl);
  array[1] = sf::Vertex(real_transform.transformPoint(tr), tex_tr);
  array[2] = sf::Vertex(real_transform.transformPoint(bl), tex_bl);
  array[3] = sf::Vertex(real_transform.transformPoint(br), tex_br);
  draw_target.draw(array, &m_texture);

  transform_stack.exitCoordSystem();
}

} // namespace gui
