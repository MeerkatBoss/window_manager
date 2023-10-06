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
#include "gui/widget.h"
#include "gui/widget_decorator.h"
#include "math/transform.h"
#include "math/vec.h"

namespace gui
{

class WidgetView : public WidgetDecorator
{
public:
  WidgetView(Widget* widget,
             const math::Point& position = math::Point(),
             const math::Vec&   scale = math::Vec(1, 1)) :
    WidgetDecorator(math::Transform(position, scale), widget)
  {
  }

  void setViewPosition(const math::Point& position)
  {
    getDecorated()->transform().setPosition(-position);
  }

  math::Point getViewPosition() const
  {
    return -getDecorated()->transform().getPosition();
  }

  void setViewScale(const math::Vec& scale)
  {
    getDecorated()->transform().setScale(scale);
  }

  math::Vec getViewScale() const
  {
    return getDecorated()->transform().getScale();
  }

  void moveView(const math::Vec& offset)
  {
    getDecorated()->transform().move(-offset);
  }

  void zoomView(double zoom)
  {
    getDecorated()->transform().scale(math::Vec(zoom, zoom));
  }

  virtual void draw(sf::RenderTarget& draw_target,
                    math::TransformStack& transform_stack) override;

private:
  sf::RenderTexture m_viewTexture;
};

} // namespace gui

#endif /* widget_view.h */
