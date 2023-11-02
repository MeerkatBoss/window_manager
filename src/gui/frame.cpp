#include "gui/frame.h"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <cstdio>

#include "math/vec.h"

namespace gui
{

Frame::Frame(const layout::Length& width, Widget* widget,
             const sf::Texture& button_texture) :
    WidgetContainer(widget->getLayoutBox()->copy()),
    m_moving(false),
    m_resizing(false),
    m_lastPos()
{
  layout::DefaultBox* main_box =
      new layout::DefaultBox(100_per, 100_per, layout::Align::Center);
  main_box->setPadding(width);
  widget->setLayoutBox(main_box);
  addWidget(widget);

  layout::DefaultBox* button_box =
      new layout::DefaultBox(width, width, layout::Align::BottomRight);
  Button* resize = new Button(*this, button_texture, button_box);
  addWidget(resize);
}

bool Frame::onMousePressed(event::MouseKey mouse_button)
{
  bool handled = Base::onMousePressed(mouse_button);
  if (handled)
  {
    return true;
  }

  if (mouse_button != event::MouseKey::Left || !isFocused())
    return false;

  m_moving = true;

  return true;
}

bool Frame::onMouseReleased(event::MouseKey mouse_button)
{
  bool handled = Base::onMouseReleased(mouse_button);

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
    getLayoutBox()->setPosition(parent_position - m_lastPos);

    return true;
  }

  m_lastPos = local_position;
  if (m_resizing)
  {
    const math::Vec size = getSize();
    const math::Vec origin(getLayoutBox()->getLocalOrigin().x * size.x,
                           getLayoutBox()->getLocalOrigin().y * size.y);

    bool success = getLayoutBox()->setSize(local_position + origin);
    if (success)
    {
      size_t widget_count = getWidgets().getSize();
      for (size_t i = 0; i < widget_count; ++i)
      {
        getWidgets()[i]->onLayoutUpdate(*getLayoutBox());
      }
    }
    return true;
  }

  return isFocused() || handled;
}

void Frame::draw(sf::RenderTarget&     draw_target,
                 math::TransformStack& transform_stack)
{
  const auto [tl, tr, bl, br] = layout::getRect(getLayoutBox()->getSize());
  const math::Vec origin = layout::getAbsoluteOrigin(getLayoutBox());

  transform_stack.enterCoordSystem(getLocalTransform());
  const math::Transform& real_transform = transform_stack.getCoordSystem();

  sf::VertexArray array(sf::TriangleStrip, 4);
  array[0] =
      sf::Vertex(real_transform.transformPoint(tl - origin), sf::Color::Blue);
  array[1] =
      sf::Vertex(real_transform.transformPoint(tr - origin), sf::Color::Blue);
  array[2] =
      sf::Vertex(real_transform.transformPoint(bl - origin), sf::Color::Blue);
  array[3] =
      sf::Vertex(real_transform.transformPoint(br - origin), sf::Color::Blue);
  draw_target.draw(array);

  transform_stack.exitCoordSystem();

  Base::draw(draw_target, transform_stack);
}

} // namespace gui
