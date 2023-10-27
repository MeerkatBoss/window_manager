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
#include "gui/layout/default_box.h"
#include "gui/layout/units.h"
#include "gui/slider.h"
#include "gui/widget.h"
#include "gui/widget_container.h"
#include "gui/widget_view.h"
#include "math/transform.h"
#include "math/vec.h"

namespace gui
{

class Scrollbar : public WidgetContainer,
                  private ButtonController,
                  private SliderController
{
  using Base = WidgetContainer;

public:
  Scrollbar(layout::Length width, Widget* widget,
            const sf::Texture& button_texture) :
      WidgetContainer(widget->getLayoutBox()->copy()),
      m_view(new WidgetView(widget, 2)),
      m_offset(.5, .5)
  {
    const layout::Length width2(2 * width.value, width.unit);

    layout::DefaultBox* view_box =
        new layout::DefaultBox(100_per, 100_per, layout::Align::TopLeft);
    view_box->setPadding(0_px, width, 0_px, width);
    m_view->setLayoutBox(view_box);

    layout::DefaultBox* up_box =
        new layout::DefaultBox(width, width, layout::Align::TopRight);
    Button* up = new Button(*this, button_texture, up_box);

    layout::DefaultBox* down_box =
        new layout::DefaultBox(width, width, layout::Align::BottomRight);
    Button* down = new Button(*this, button_texture, down_box);

    layout::DefaultBox* left_box =
        new layout::DefaultBox(width, width, layout::Align::BottomLeft);
    Button* left = new Button(*this, button_texture, left_box);

    layout::DefaultBox* right_box =
        new layout::DefaultBox(width2, width, layout::Align::BottomRight);
    right_box->setPadding(0_px, 0_px, 0_px, width);
    Button* right = new Button(*this, button_texture, right_box);

    layout::DefaultBox* vert_box =
        new layout::DefaultBox(width, 100_per, layout::Align::TopRight);
    vert_box->setPadding(width, 0_px);
    Slider* vertical = new Slider(*this, vert_box, math::Vec(1, 0.1));

    layout::DefaultBox* horiz_box =
        new layout::DefaultBox(100_per, width, layout::Align::BottomLeft);
    horiz_box->setPadding(0_px, 0_px, width, width2);
    Slider* horizontal = new Slider(*this, horiz_box, math::Vec(0.1, 1));

    m_buttonUp    = up->getId();
    m_buttonDown  = down->getId();
    m_buttonLeft  = left->getId();
    m_buttonRight = right->getId();

    m_scrollVert  = vertical->getId();
    m_scrollHoriz = horizontal->getId();

    addWidget(m_view);
    addWidget(vertical);
    addWidget(horizontal);
    addWidget(up);
    addWidget(down);
    addWidget(left);
    addWidget(right);

    updateViewPosition();
  }

  virtual void onClick(size_t button_id) override;

  virtual void onHold(size_t button_id, double delta_time) override;

  virtual void setValue(size_t slider_id, const math::Vec& val) override;

  virtual math::Vec getValue(size_t slider_id) override;

private:
  void updateViewPosition();

  math::Vec   m_localWidth;
  WidgetView* m_view;
  size_t m_buttonUp, m_buttonDown, m_buttonLeft, m_buttonRight, m_scrollVert,
      m_scrollHoriz;
  math::Vec m_offset;
};

} // namespace gui

#endif /* scrollbar.h */
