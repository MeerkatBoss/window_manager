/**
 * @file widget_view.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-10-13
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_WIDGET_VIEW_H
#define __GUI_WIDGET_VIEW_H

#include <SFML/Graphics/RenderTexture.hpp>

#include "gui/layout/default_box.h"
#include "gui/layout/units.h"
#include "gui/widget.h"
#include "gui/widget_container.h"
#include "math/transform.h"
#include "math/vec.h"

namespace gui
{

// TODO: Replace inheritance with composition
class WidgetView : public WidgetContainer
{
public:
  explicit WidgetView(Widget* widget, double zoom = 1) :
      WidgetContainer(widget->getLayoutBox()->copy()),
      m_widgetTransform(math::Point(), math::Vec(zoom, zoom))
  {
    layout::DefaultBox* widget_box =
      new layout::DefaultBox(layout::Length(getSize().x, layout::Unit::Pixel),
                             layout::Length(getSize().y, layout::Unit::Pixel),
                             layout::Align::Center);
    widget->setLayoutBox(widget_box);
    addWidget(widget);
  }

  void setViewPosition(const math::Point& position);

  math::Point getViewPosition() const;

  void setViewScale(const math::Vec& scale)
  {
    m_widgetTransform.setScale(scale);
  }

  math::Vec getViewScale() const
  {
    return m_widgetTransform.getScale();
  }

  void moveView(const math::Vec& offset)
  {
    m_widgetTransform.move(-offset);
  }

  void zoomView(double zoom)
  {
    m_widgetTransform.scale(math::Vec(zoom, zoom));
  }

  virtual bool onMouseMoved(const math::Vec&      position,
                            math::TransformStack& transform_stack) override;

  virtual void draw(sf::RenderTarget&     draw_target,
                    math::TransformStack& transform_stack) override;

  virtual void onLayoutUpdate(const layout::LayoutBox& parent_box) override
  {
    const math::Point offset = getViewPosition();
    WidgetContainer::onLayoutUpdate(parent_box);
    setViewPosition(offset);
  }

private:
  Widget*       getDecorated() { return getWidgets()[0]; }
  const Widget* getDecorated() const { return getWidgets()[0]; }

  math::Transform   m_widgetTransform;
  sf::RenderTexture m_viewTexture;
};

} // namespace gui

#endif /* widget_view.h */
