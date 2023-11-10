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

#include "Event/Event.h"
#include "GUI/Layout/DefaultBox.h"
#include "GUI/Layout/Units.h"
#include "GUI/Widget.h"
#include "GUI/WidgetContainer.h"
#include "Math/Transform.h"
#include "Math/Vec.h"

namespace gui
{

// TODO: Replace inheritance with composition
class WidgetView : public Widget
{
public:
  explicit WidgetView(Widget* widget, double zoom = 1);

  virtual ~WidgetView() { delete m_widget; }

  void setViewPosition(const math::Point& position);

  math::Point getViewPosition() const;

  void setViewScale(const math::Vec& scale)
  {
    m_widgetTransform.setScale(scale);
  }

  math::Vec getViewScale() const { return m_widgetTransform.getScale(); }

  void moveView(const math::Vec& offset) { m_widgetTransform.move(-offset); }

  void zoomView(double zoom) { m_widgetTransform.scale(math::Vec(zoom, zoom)); }

  virtual bool onEvent(const event::Event& event) override;

  virtual void draw(sf::RenderTarget&     draw_target,
                    math::TransformStack& transform_stack) override;

  virtual void onLayoutUpdate(const layout::LayoutBox& parent_box) override
  {
    const math::Point offset = getViewPosition();
    getLayoutBox().updateParent(parent_box);
    m_widget->onLayoutUpdate(parent_box);
    setViewPosition(offset);
  }

private:
  bool forwardMouseMoved(const event::MouseMoveEvent& event);

  bool forwardMouseButton(const event::MouseButtonEvent& event);

  Widget*           m_widget;
  math::Transform   m_widgetTransform;
  sf::RenderTexture m_viewTexture;
};

} // namespace gui

#endif /* WidgetView.h */
