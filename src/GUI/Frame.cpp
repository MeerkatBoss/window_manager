#include "GUI/Frame.h"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <cstdio>

#include "GUI/Layout/DefaultBox.h"
#include "GUI/Layout/Units.h"
#include "GUI/WidgetContainer.h"
#include "Math/Vec.h"

namespace gui
{

Frame::Frame(const layout::Length& width, Widget* widget,
             const sf::Texture& button_texture) :
    Widget(widget->getLayoutBox()),
    m_container(layout::DefaultBox(100_per, 100_per, layout::Align::Center)),
    m_moving(false),
    m_resizing(false),
    m_lastPos()
{
  layout::DefaultBox main_box(100_per, 100_per, layout::Align::Center);
  main_box.setPadding(width);
  widget->setLayoutBox(main_box);
  m_container.addWidget(widget);

  layout::DefaultBox button_box(width, width, layout::Align::BottomRight);
  Button*            resize = new Button(*this, button_texture, button_box);
  m_container.addWidget(resize);
}

void Frame::onLayoutUpdate(const layout::LayoutBox& parent_box)
{
  getLayoutBox().updateParent(parent_box);
  m_container.onLayoutUpdate(getLayoutBox());
}

bool Frame::onEvent(const event::Event& event)
{
  if (event.isPositionalEvent())
  {
    event.asPositionalEvent()->getTransformStack()
      .enterCoordSystem(getLocalTransform());
  }
  bool handled = m_container.onEvent(event);
  if (event.isPositionalEvent())
  {
    event.asPositionalEvent()->getTransformStack()
      .exitCoordSystem();
  }

  return handled || Widget::onEvent(event);
}


bool Frame::onMousePressed(const math::Vec&      position,
                           event::MouseKey       mouse_button,
                           math::TransformStack& transform_stack)
{
  if (mouse_button == event::MouseKey::Left &&
      containsPoint(position, transform_stack))
  {
    m_moving = true;
    return true;
  }

  return false;
}

bool Frame::onMouseReleased(const math::Vec&, event::MouseKey mouse_button,
                            math::TransformStack&)
{
  if (mouse_button == event::MouseKey::Left)
  {
    m_moving = false;
  }

  return false;
}

bool Frame::onMouseMoved(const math::Vec&      position,
                         math::TransformStack& transform_stack)
{
  transform_stack.enterCoordSystem(getLocalTransform());

  const math::Vec local_position =
      transform_stack.getCoordSystem().restorePoint(position);

  transform_stack.exitCoordSystem();

  const math::Vec parent_position =
      transform_stack.getCoordSystem().restorePoint(position);

  if (m_moving)
  {
    getLayoutBox().setPosition(parent_position - m_lastPos);

    return false;
  }

  m_lastPos = local_position;
  if (m_resizing)
  {
    const math::Vec size = getSize();

    bool success = getLayoutBox().setSize(local_position);
    if (success)
    {
      m_container.onLayoutUpdate(getLayoutBox());
    }
    return false;
  }

  return false;
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
  m_container.draw(draw_target, transform_stack);

  transform_stack.exitCoordSystem();
}

} // namespace gui
