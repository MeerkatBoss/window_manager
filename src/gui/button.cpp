#include "gui/button.h"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

#include "event/keys.h"
#include "gui/layout/layout_box.h"
#include "gui/widget.h"
#include "math/transform.h"
#include "math/vec.h"

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

bool Button::onMouseMoved(const math::Vec&      position,
                          math::TransformStack& transform_stack)
{
  return m_hovered = containsPoint(position, transform_stack);
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

void Button::draw(sf::RenderTarget&     draw_target,
                  math::TransformStack& transform_stack)
{
  const auto [tl, tr, bl, br] = layout::getRect(getLayoutBox()->getSize());

  const math::Vec tex_size(m_texture.getSize().x, m_texture.getSize().y);
  const auto [tex_tl, tex_tr, tex_bl, tex_br] = layout::getRect(tex_size);

  const math::Vec origin = layout::getAbsoluteOrigin(getLayoutBox());

  transform_stack.enterCoordSystem(getLocalTransform());
  const math::Transform& real_transform = transform_stack.getCoordSystem();

  sf::VertexArray array(sf::TriangleStrip, 4);
  array[0] = sf::Vertex(real_transform.transformPoint(tl - origin), tex_tl);
  array[1] = sf::Vertex(real_transform.transformPoint(tr - origin), tex_tr);
  array[2] = sf::Vertex(real_transform.transformPoint(bl - origin), tex_bl);
  array[3] = sf::Vertex(real_transform.transformPoint(br - origin), tex_br);
  draw_target.draw(array, &m_texture);

  transform_stack.exitCoordSystem();
}

} // namespace gui
