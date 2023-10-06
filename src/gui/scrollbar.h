/**
 * @file scrollbar.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 * 
 * @brief
 *
 * @version 0.1
 * @date 2023-10-06
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_SCROLLBAR_H
#define __GUI_SCROLLBAR_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "gui/button.h"
#include "gui/slider.h"
#include "gui/widget.h"
#include "gui/widget_decorator.h"
#include "gui/widget_view.h"
#include "math/transform.h"
#include "math/vec.h"

namespace gui
{

class Scrollbar : public WidgetDecorator, private ButtonController,
                                          private SliderController
{
using Base = WidgetDecorator;
public:
  Scrollbar(const math::Transform& transform,
            double scroll_bar_width,
            Widget* widget, 
            const sf::Texture& button_texture) :
    WidgetDecorator(transform, new WidgetView(widget, transform.getPosition(),
                                                      transform.getScale())),
    m_localWidth(scroll_bar_width / transform.getScale().x,
                 scroll_bar_width / transform.getScale().y),
    m_view(static_cast<WidgetView*>(getDecorated())),
    m_buttonUp(*this, button_texture,
               math::Point( .5 - m_localWidth.x/2,
                           -.5 + m_localWidth.y/2),
               m_localWidth),
    m_buttonDown(*this, button_texture,
               math::Point( .5 - m_localWidth.x/2,
                            .5 - m_localWidth.y/2),
               m_localWidth),
    m_buttonLeft(*this, button_texture,
               math::Point(-.5 + m_localWidth.x/2,
                            .5 - m_localWidth.y/2),
               m_localWidth),
    m_buttonRight(*this, button_texture,
               math::Point( .5 - 3*m_localWidth.x/2,
                            .5 -   m_localWidth.y/2),
               m_localWidth),
    m_vertical(*this,
               math::Transform(
                  math::Point( .5 - m_localWidth.x/2, 0),
                  math::Vec(m_localWidth.x, 1 - 2*m_localWidth.y)),
               math::Vec(1, 0.1)),
    m_horizontal(*this,
               math::Transform(
                  math::Point( -m_localWidth.x/2,
                               .5 -   m_localWidth.y/2),
                  math::Vec(1 - 3*m_localWidth.x, m_localWidth.y)),
               math::Vec(0.1, 1)),
    m_offset(0, 0)
  {
    m_view->transform().move(-m_localWidth/2);
    m_view->transform().scale(
        math::Vec(1 / (1 + m_localWidth.x),
                  1 / (1 + m_localWidth.y)));
  }
    
  virtual bool onEvent(const event::Event& event) override
  {
    if (isFocused() && needEventForward(event))
    {
      bool handled = false;
      if (!handled)
        handled = m_buttonUp.onEvent(event);
      if (!handled)
        handled = m_buttonDown.onEvent(event);
      if (!handled)
        handled = m_buttonLeft.onEvent(event);
      if (!handled)
        handled = m_buttonRight.onEvent(event);
      if (!handled)
        handled = m_vertical.onEvent(event);
      if (!handled)
        handled = m_horizontal.onEvent(event);

      if (handled)
        return true;
    }

    return Base::onEvent(event);
  }

  virtual bool onMouseMoved(const math::Vec& position,
                            math::TransformStack& transform_stack) override;

  virtual bool onMouseReleased(event::MouseKey mouse_button) override;

  virtual bool onUpdate(double delta_time) override;

  virtual void onClick(size_t button_id) override;

  virtual void onHold(size_t button_id, double delta_time) override;

  virtual void draw(sf::RenderTarget& draw_target,
                    math::TransformStack& transform_stack) override;

  virtual void setValue(size_t slider_id, const math::Vec& val) override;

  virtual math::Vec getValue(size_t slider_id) override;

private:
  /*
  void drawWidget(sf::RenderTarget& draw_target,
                  math::TransformStack& transform_stack);
                  */

  math::Vec m_localWidth;
  WidgetView* m_view;
  Button m_buttonUp, m_buttonDown, m_buttonLeft, m_buttonRight;
  Slider m_vertical, m_horizontal;
  // sf::RenderTexture m_renderTexture;
  math::Vec m_offset;
};

} // namespace gui

#endif /* scrollbar.h */
