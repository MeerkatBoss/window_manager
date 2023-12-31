#include "GUI/Scrollbar.h"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "Assets/AssetManager.h"
#include "GUI/Layout/DefaultBox.h"
#include "GUI/Layout/Units.h"

namespace gui
{

Scrollbar::Scrollbar(layout::Length width, Widget* widget) :
    Widget(widget->getLayoutBox()),
    m_container(layout::DefaultBox(100_per, 100_per, layout::Align::Center)),
    m_view(new WidgetView(widget, 2)),
    m_offset(.5, .5)
{
  const sf::Texture& button_texture = assets::AssetManager::getButtonTexture();
  const layout::Length width2(2 * width.value, width.unit);

  layout::DefaultBox view_box(100_per, 100_per, layout::Align::TopLeft);
  view_box.setPadding(0_px, width, 0_px, width);
  m_view->setLayoutBox(view_box);

  layout::DefaultBox up_box(width, width, layout::Align::TopRight);
  Button*            up = new Button(*this, button_texture, up_box, "^");

  layout::DefaultBox down_box(width, width, layout::Align::BottomRight);
  Button*            down = new Button(*this, button_texture, down_box, "v");

  layout::DefaultBox left_box(width, width, layout::Align::BottomLeft);
  Button*            left = new Button(*this, button_texture, left_box, "<");

  layout::DefaultBox right_box(width2, width, layout::Align::BottomRight);
  right_box.setPadding(0_px, 0_px, 0_px, width);
  Button* right = new Button(*this, button_texture, right_box, ">");

  layout::DefaultBox vert_box(width, 100_per, layout::Align::TopRight);
  vert_box.setPadding(width, 0_px);
  Slider* vertical = new Slider(*this, vert_box, math::Vec(1, 0.1));

  layout::DefaultBox horiz_box(100_per, width, layout::Align::BottomLeft);
  horiz_box.setPadding(0_px, 0_px, width, width2);
  Slider* horizontal = new Slider(*this, horiz_box, math::Vec(0.1, 1));

  m_buttonUp    = up->getId();
  m_buttonDown  = down->getId();
  m_buttonLeft  = left->getId();
  m_buttonRight = right->getId();

  m_scrollVert  = vertical->getId();
  m_scrollHoriz = horizontal->getId();

  m_container.addWidget(m_view);
  m_container.addWidget(vertical);
  m_container.addWidget(horizontal);
  m_container.addWidget(up);
  m_container.addWidget(down);
  m_container.addWidget(left);
  m_container.addWidget(right);

  updateViewPosition();
}

void Scrollbar::updateViewPosition()
{
  if (m_offset.x < 0)
    m_offset.x = 0;
  if (m_offset.x > 1)
    m_offset.x = 1;
  if (m_offset.y < 0)
    m_offset.y = 0;
  if (m_offset.y > 1)
    m_offset.y = 1;

  m_view->setViewPosition(math::Vec(m_offset.x, m_offset.y));
}

void Scrollbar::onClick(size_t button_id)
{
  if (button_id == m_buttonUp)
  {
    m_offset.y -= 0.01;
  }
  if (button_id == m_buttonDown)
  {
    m_offset.y += 0.01;
  }
  if (button_id == m_buttonLeft)
  {
    m_offset.x -= 0.01;
  }
  if (button_id == m_buttonRight)
  {
    m_offset.x += 0.01;
  }

  updateViewPosition();
}

void Scrollbar::onHold(size_t button_id, double delta_time)
{
  if (button_id == m_buttonUp)
  {
    m_offset.y -= 0.1 * delta_time;
  }
  if (button_id == m_buttonDown)
  {
    m_offset.y += 0.1 * delta_time;
  }
  if (button_id == m_buttonLeft)
  {
    m_offset.x -= 0.1 * delta_time;
  }
  if (button_id == m_buttonRight)
  {
    m_offset.x += 0.1 * delta_time;
  }

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

void Scrollbar::onLayoutUpdate(const layout::LayoutBox& parent_box)
{
  getLayoutBox().updateParent(parent_box);
  m_container.onLayoutUpdate(getLayoutBox());
}

bool Scrollbar::onEvent(const event::Event& event)
{
  if (event.isPositionalEvent())
  {
    event.asPositionalEvent()->getTransformStack().enterCoordSystem(
        getLocalTransform());
  }
  bool handled = m_container.onEvent(event);
  if (event.isPositionalEvent())
  {
    event.asPositionalEvent()->getTransformStack().exitCoordSystem();
  }

  return handled || Widget::onEvent(event);
}

void Scrollbar::draw(sf::RenderTarget&     draw_target,
                     math::TransformStack& transform_stack)
{
  transform_stack.enterCoordSystem(getLocalTransform());
  m_container.draw(draw_target, transform_stack);
  transform_stack.exitCoordSystem();
}

} // namespace gui
