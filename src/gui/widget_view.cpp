#include "gui/widget_view.h"

#include <cstdio>
#include <math.h>

#include "math/vec.h"

namespace gui
{
WidgetView::WidgetView(Widget* widget, double zoom) :
    WidgetContainer(widget->getLayoutBox()->copy()),
    m_widgetTransform(math::Point(), math::Vec(zoom, zoom))
{
  layout::DefaultBox* widget_box = new layout::DefaultBox(
      layout::Length(getSize().x, layout::Unit::Pixel),
      layout::Length(getSize().y, layout::Unit::Pixel), layout::Align::Center);
  widget->setLayoutBox(widget_box);
  addWidget(widget);
}

void WidgetView::setViewPosition(const math::Point& position)
{
  const math::Vec size = getSize();
  const math::Vec widget_size =
      m_widgetTransform.transformVector(getDecorated()->getSize());

  const math::Vec unit = size - widget_size;
  const math::Vec offset(position.x * fabs(unit.x), position.y * fabs(unit.y));

  m_widgetTransform.setOffset(-offset);
}

math::Point WidgetView::getViewPosition() const
{
  const math::Vec size = getSize();
  const math::Vec widget_size =
      m_widgetTransform.transformVector(getDecorated()->getSize());

  const math::Vec unit   = size - widget_size;
  const math::Vec offset = m_widgetTransform.getOffset();

  return -math::Point(offset.x / fabs(unit.x), offset.y / fabs(unit.y));
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

  return Widget::onMouseMoved(position, transform_stack) || handled;
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
  const math::Vec origin      = layout::getAbsoluteOrigin(getLayoutBox());

  const math::Transform& real_transform = transform_stack.getCoordSystem();

  const math::Point real_tl = real_transform.transformPoint(tl - origin);
  const math::Point real_tr = real_transform.transformPoint(tr - origin);
  const math::Point real_bl = real_transform.transformPoint(bl - origin);
  const math::Point real_br = real_transform.transformPoint(br - origin);

  sf::VertexArray array(sf::TriangleStrip, 4);
  array[0] = sf::Vertex(real_tl, real_tl);
  array[1] = sf::Vertex(real_tr, real_tr);
  array[2] = sf::Vertex(real_bl, real_bl);
  array[3] = sf::Vertex(real_br, real_br);
  draw_target.draw(array, &m_viewTexture.getTexture());

  transform_stack.exitCoordSystem();
}

} // namespace gui
