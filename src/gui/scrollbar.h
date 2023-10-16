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
#include "gui/widget_container.h"
#include "gui/widget_view.h"
#include "math/transform.h"
#include "math/vec.h"

namespace gui
{

class Scrollbar : public WidgetContainer, private ButtonController,
                                          private SliderController
{
using Base = WidgetContainer;
public:
  Scrollbar(const math::Transform& transform,
            double scroll_bar_width,
            Widget* widget, 
            const sf::Texture& button_texture) :
    WidgetContainer(transform),
    m_localWidth(scroll_bar_width / transform.getScale().x,
                 scroll_bar_width / transform.getScale().y),
    m_view(new WidgetView(widget, -m_localWidth/2, transform.getScale())),
    m_offset(0, 0)
  {
    m_view->transform().scale(
        math::Vec(1 / (1 + m_localWidth.x),
                  1 / (1 + m_localWidth.y)));
    Button* up = new Button(*this, button_texture,
                            math::Point( .5 - m_localWidth.x/2,
                                        -.5 + m_localWidth.y/2),
                            m_localWidth);
    Button* down = new Button(*this, button_texture,
               math::Point( .5 - m_localWidth.x/2,
                            .5 - m_localWidth.y/2),
               m_localWidth);
    Button* left = new Button(*this, button_texture,
               math::Point(-.5 + m_localWidth.x/2,
                            .5 - m_localWidth.y/2),
               m_localWidth);
    Button* right = new Button(*this, button_texture,
               math::Point( .5 - 3*m_localWidth.x/2,
                            .5 -   m_localWidth.y/2),
               m_localWidth);
    Slider* vertical = new Slider(*this,
               math::Transform(
                  math::Point( .5 - m_localWidth.x/2, 0),
                  math::Vec(m_localWidth.x, 1 - 2*m_localWidth.y)),
               math::Vec(1, 0.1));
    Slider* horizontal = new Slider(*this,
               math::Transform(
                  math::Point( -m_localWidth.x/2,
                               .5 -   m_localWidth.y/2),
                  math::Vec(1 - 3*m_localWidth.x, m_localWidth.y)),
               math::Vec(0.1, 1));

    m_buttonUp = up->getId();
    m_buttonDown = down->getId();
    m_buttonLeft = left->getId();
    m_buttonRight = right->getId();

    m_scrollVert = vertical->getId();
    m_scrollHoriz = horizontal->getId();

    addWidget(m_view);
    addWidget(vertical);
    addWidget(horizontal);
    addWidget(up);
    addWidget(down);
    addWidget(left);
    addWidget(right);
  }

  virtual void onClick(size_t button_id) override;

  virtual void onHold(size_t button_id, double delta_time) override;

  virtual void setValue(size_t slider_id, const math::Vec& val) override;

  virtual math::Vec getValue(size_t slider_id) override;

private:
  void updateViewPosition();

  math::Vec m_localWidth;
  WidgetView* m_view;
  size_t m_buttonUp, m_buttonDown, m_buttonLeft, m_buttonRight,
         m_scrollVert, m_scrollHoriz;
  math::Vec m_offset;
};

} // namespace gui

#endif /* scrollbar.h */
