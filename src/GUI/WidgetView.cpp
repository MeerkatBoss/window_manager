#include "GUI/WidgetView.h"

#include <cstdio>
#include <math.h>

#include "Math/Vec.h"

namespace gui
{
WidgetView::WidgetView(Widget* widget, double zoom) :
    WidgetContainer(widget->getLayoutBox()->clone()),
    m_widgetTransform(math::Point(), math::Vec(zoom, zoom))
{
  layout::DefaultBox* widget_box = new layout::DefaultBox(
      layout::Length(getSize().x, layout::Unit::Pixel),
      layout::Length(getSize().y, layout::Unit::Pixel), layout::Align::TopLeft);
  widget->setLayoutBox(widget_box);
  addWidget(widget);
}

void WidgetView::setViewPosition(const math::Point& position)
{
  const math::Vec size = getSize();
  const math::Vec widget_size =
      m_widgetTransform.transformVector(getDecorated()->getSize());

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
      m_widgetTransform.transformVector(getDecorated()->getSize());

  const math::Vec unit   = size - widget_size;
  const math::Vec offset = m_widgetTransform.getOffset();

  return math::Point(
      .5 - (offset.x - size.x / 2 + widget_size.x / 2) / fabs(unit.x),
      .5 - (offset.y - size.y / 2 + widget_size.y / 2) / fabs(unit.y));
}

bool WidgetView::onMousePressed(const math::Vec&      position,
                                event::MouseKey       mouse_button,
                                math::TransformStack& transform_stack)
{
  if (containsPoint(position, transform_stack))
  {
    transform_stack.enterCoordSystem(getLocalTransform());
    transform_stack.enterCoordSystem(m_widgetTransform);

    bool handled =
        getDecorated()->onMousePressed(position, mouse_button, transform_stack);

    transform_stack.exitCoordSystem();
    transform_stack.exitCoordSystem();

    return handled;
  }

  return false;
}

bool WidgetView::onMouseReleased(const math::Vec&      position,
                                 event::MouseKey       mouse_button,
                                 math::TransformStack& transform_stack)
{
  transform_stack.enterCoordSystem(getLocalTransform());
  transform_stack.enterCoordSystem(m_widgetTransform);

  bool handled =
      getDecorated()->onMouseReleased(position, mouse_button, transform_stack);

  transform_stack.exitCoordSystem();
  transform_stack.exitCoordSystem();

  return handled;
}

bool WidgetView::onMouseMoved(const math::Vec&      position,
                              math::TransformStack& transform_stack)
{
  bool focused = containsPoint(position, transform_stack);
  if (focused)
  {
    focus();
  }
  else
  {
    unfocus();
  }

  transform_stack.enterCoordSystem(getLocalTransform());
  transform_stack.enterCoordSystem(m_widgetTransform);

  bool handled = getDecorated()->onMouseMoved(position, transform_stack);

  transform_stack.exitCoordSystem();
  transform_stack.exitCoordSystem();

  return handled;
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
  getDecorated()->draw(m_viewTexture, transform_stack);
  m_viewTexture.display();
  transform_stack.exitCoordSystem();

  const auto [tl, tr, bl, br] = layout::getRect(getLayoutBox()->getSize());

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
