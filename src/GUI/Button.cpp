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

void Button::onLayoutUpdate(const layout::LayoutBox& parent_box)
{
  Widget::onLayoutUpdate(parent_box);

  m_textLayoutBox.updateParent(getLayoutBox());
  const math::Vec text_size = m_textLayoutBox.getSize();
  m_text.setCharacterSize(text_size.y * .8);

  const double text_width = m_text.getGlobalBounds().width;
  m_text.setOrigin(text_width / 2, 0);
  m_text.setPosition(m_textLayoutBox.getPosition());
}

bool Button::onMousePressed(const math::Vec&      position,
                            event::MouseKey       mouse_button,
                            math::TransformStack& transform_stack)
{
  onMouseMoved(position, transform_stack);

  if (mouse_button != event::MouseKey::Left || !m_hovered)
    return false;

  m_controller.onClick(getId());
  m_pressed = true;

  return true;
}

bool Button::onMouseReleased(const math::Vec&      position,
                             event::MouseKey       mouse_button,
                             math::TransformStack& transform_stack)
{
  onMouseMoved(position, transform_stack);

  if (mouse_button != event::MouseKey::Left || !m_pressed)
    return false;

  m_controller.onRelease(getId());
  m_pressed = false;

  return false;
}

bool Button::onMouseMoved(const math::Vec&      position,
                          math::TransformStack& transform_stack)
{
  m_hovered = containsPoint(position, transform_stack);

  return false;
}

bool Button::onTick(double delta_time)
{
  if (m_pressed)
  {
    m_controller.onHold(getId(), delta_time);
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

  sf::Text drawn_text = m_text;
  drawn_text.setScale(real_transform.getScale());
  drawn_text.setPosition(real_transform.transformPoint(m_text.getPosition()));
  draw_target.draw(drawn_text);

  transform_stack.exitCoordSystem();
}

} // namespace gui
