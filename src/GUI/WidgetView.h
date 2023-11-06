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

#include "GUI/Layout/DefaultBox.h"
#include "GUI/Layout/Units.h"
#include "GUI/Widget.h"
#include "GUI/WidgetContainer.h"
#include "Math/Transform.h"
#include "Math/Vec.h"

namespace gui
{

// TODO: Replace inheritance with composition
class WidgetView : public WidgetContainer
{
public:
  explicit WidgetView(Widget* widget, double zoom = 1);

  void setViewPosition(const math::Point& position);

  math::Point getViewPosition() const;

  void setViewScale(const math::Vec& scale)
  {
    m_widgetTransform.setScale(scale);
  }

  math::Vec getViewScale() const { return m_widgetTransform.getScale(); }

  void moveView(const math::Vec& offset) { m_widgetTransform.move(-offset); }

  void zoomView(double zoom) { m_widgetTransform.scale(math::Vec(zoom, zoom)); }

  virtual bool onMousePressed(const math::Vec&      position,
                              event::MouseKey       mouse_button,
                              math::TransformStack& transform_stack) override;

  virtual bool onMouseReleased(const math::Vec&      position,
                               event::MouseKey       mouse_button,
                               math::TransformStack& transform_stack) override;

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

#endif /* WidgetView.h */
