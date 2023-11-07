#include "GUI/WidgetView.h"

#include <cstdio>
#include <math.h>

#include "Event/Event.h"
#include "Event/Keys.h"
#include "Math/TransformStack.h"
#include "Math/Vec.h"

namespace gui
{
WidgetView::WidgetView(Widget* widget, double zoom) :
    Widget(widget->getLayoutBox()),
    m_widget(widget),
    m_widgetTransform(math::Point(), math::Vec(zoom, zoom))
{
  layout::DefaultBox widget_box(
      layout::Length(getSize().x, layout::Unit::Pixel),
      layout::Length(getSize().y, layout::Unit::Pixel), layout::Align::Free);
  m_widget->setLayoutBox(widget_box);
}

void WidgetView::setViewPosition(const math::Point& position)
{
  const math::Vec size = getSize();
  const math::Vec widget_size =
      m_widgetTransform.transformVector(m_widget->getSize());

  const math::Vec unit = size - widget_size;
  const math::Vec offset(
      (.5 - position.x) * fabs(unit.x) + size.x / 2 - widget_size.x / 2,
      (.5 - position.y) * fabs(unit.y) + size.y / 2 - widget_size.y / 2);

  m_widgetTransform.setOffset(offset);
}

math::Point WidgetView::getViewPosition() const
{
  const math::Vec size = getSize();
  const math::Vec widget_size =
      m_widgetTransform.transformVector(m_widget->getSize());

  const math::Vec unit   = size - widget_size;
  const math::Vec offset = m_widgetTransform.getOffset();

  return math::Point(
      .5 - (offset.x - size.x / 2 + widget_size.x / 2) / fabs(unit.x),
      .5 - (offset.y - size.y / 2 + widget_size.y / 2) / fabs(unit.y));
}

bool WidgetView::onEvent(const event::Event& event)
{
  if (event.getEventType() == event::EventType::MouseMove)
  {
    return forwardMouseMoved(static_cast<const event::MouseMoveEvent&>(event));
  }
  if (event.getEventType() == event::EventType::MouseButton)
  {
    return forwardMouseButton(
        static_cast<const event::MouseButtonEvent&>(event));
  }

  return m_widget->onEvent(event);
}

bool WidgetView::forwardMouseMoved(const event::MouseMoveEvent& event)
{
  // TODO: clamp position
  math::TransformStack& transform_stack = event.transform_stack;

  transform_stack.enterCoordSystem(getLocalTransform());
  transform_stack.enterCoordSystem(m_widgetTransform);
  bool handled = m_widget->onEvent(event);
  transform_stack.exitCoordSystem();
  transform_stack.exitCoordSystem();
  return handled;
}

bool WidgetView::forwardMouseButton(const event::MouseButtonEvent& event)
{
  // TODO: clamp position
  const math::Vec&      position        = event.position;
  math::TransformStack& transform_stack = event.transform_stack;

  if (event.buttonState == event::KeyState::Released ||
      containsPoint(position, transform_stack))
  {
    transform_stack.enterCoordSystem(getLocalTransform());
    transform_stack.enterCoordSystem(m_widgetTransform);
    bool handled = m_widget->onEvent(event);
    transform_stack.exitCoordSystem();
    transform_stack.exitCoordSystem();
    return handled;
  }

  return false;
}

void WidgetView::draw(sf::RenderTarget&     draw_target,
                      math::TransformStack& transform_stack)
{
  if (m_viewTexture.getSize().x < draw_target.getSize().x ||
      m_viewTexture.getSize().y < draw_target.getSize().y)
  {
    m_viewTexture.create(draw_target.getSize().x, draw_target.getSize().y);
  }

  transform_stack.enterCoordSystem(getLocalTransform());

  transform_stack.enterCoordSystem(m_widgetTransform);
  m_viewTexture.clear(sf::Color(100, 100, 100));
  m_widget->draw(m_viewTexture, transform_stack);
  m_viewTexture.display();
  transform_stack.exitCoordSystem();

  const auto [tl, tr, bl, br] = layout::getRect(getSize());

  const math::Transform& real_transform = transform_stack.getCoordSystem();

  const math::Point real_tl = real_transform.transformPoint(tl);
  const math::Point real_tr = real_transform.transformPoint(tr);
  const math::Point real_bl = real_transform.transformPoint(bl);
  const math::Point real_br = real_transform.transformPoint(br);

  sf::VertexArray array(sf::TriangleStrip, 4);
  array[0] = sf::Vertex(real_tl, real_tl);
  array[1] = sf::Vertex(real_tr, real_tr);
  array[2] = sf::Vertex(real_bl, real_bl);
  array[3] = sf::Vertex(real_br, real_br);
  draw_target.draw(array, &m_viewTexture.getTexture());

  transform_stack.exitCoordSystem();
}

} // namespace gui
