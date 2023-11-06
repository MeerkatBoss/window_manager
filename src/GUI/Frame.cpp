#include "GUI/Frame.h"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <cstdio>

#include "Math/Vec.h"

namespace gui
{

Frame::Frame(const layout::Length& width, Widget* widget,
             const sf::Texture& button_texture) :
    WidgetContainer(widget->getLayoutBox()),
    m_moving(false),
    m_resizing(false),
    m_lastPos()
{
  layout::DefaultBox main_box(100_per, 100_per, layout::Align::Center);
  main_box.setPadding(width);
  widget->setLayoutBox(main_box);
  addWidget(widget);

  layout::DefaultBox button_box(width, width, layout::Align::BottomRight);
  Button* resize = new Button(*this, button_texture, button_box);
  addWidget(resize);
}

bool Frame::onMousePressed(const math::Vec&      position,
                           event::MouseKey       mouse_button,
                           math::TransformStack& transform_stack)
{
  bool handled = Base::onMousePressed(position, mouse_button, transform_stack);
  if (handled)
  {
    return true;
  }

  if (mouse_button == event::MouseKey::Left &&
      containsPoint(position, transform_stack))
  {
    m_moving = true;
    return true;
  }

  return false;
}

bool Frame::onMouseReleased(const math::Vec&      position,
                            event::MouseKey       mouse_button,
                            math::TransformStack& transform_stack)
{
  bool handled = Base::onMouseReleased(position, mouse_button, transform_stack);

  if (mouse_button != event::MouseKey::Left || !m_moving)
    return handled;

  m_moving = false;
  return true;
}

bool Frame::onMouseMoved(const math::Vec&      position,
                         math::TransformStack& transform_stack)
{
  bool handled = Base::onMouseMoved(position, transform_stack);

  transform_stack.enterCoordSystem(getLocalTransform());

  const math::Vec local_position =
      transform_stack.getCoordSystem().restorePoint(position);

  transform_stack.exitCoordSystem();

  const math::Vec parent_position =
      transform_stack.getCoordSystem().restorePoint(position);

  if (m_moving)
  {
    getLayoutBox().setPosition(parent_position - m_lastPos);

    return true;
  }

  m_lastPos = local_position;
  if (m_resizing)
  {
    const math::Vec size = getSize();

    bool success = getLayoutBox().setSize(local_position);
    if (success)
    {
      size_t widget_count = getWidgets().getSize();
      for (size_t i = 0; i < widget_count; ++i)
      {
        getWidgets()[i]->onLayoutUpdate(getLayoutBox());
      }
    }
    return true;
  }

  return isFocused() || handled;
}

void Frame::draw(sf::RenderTarget&     draw_target,
                 math::TransformStack& transform_stack)
{
  const auto [tl, tr, bl, br] = layout::getRect(getSize());

  transform_stack.enterCoordSystem(getLocalTransform());
  const math::Transform& real_transform = transform_stack.getCoordSystem();

  sf::VertexArray array(sf::TriangleStrip, 4);
  array[0] = sf::Vertex(real_transform.transformPoint(tl), sf::Color::Blue);
  array[1] = sf::Vertex(real_transform.transformPoint(tr), sf::Color::Blue);
  array[2] = sf::Vertex(real_transform.transformPoint(bl), sf::Color::Blue);
  array[3] = sf::Vertex(real_transform.transformPoint(br), sf::Color::Blue);
  draw_target.draw(array);

  transform_stack.exitCoordSystem();

  Base::draw(draw_target, transform_stack);
}

} // namespace gui
