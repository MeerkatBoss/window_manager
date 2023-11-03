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

#include "GUI/Button.h"
#include "GUI/Layout/DefaultBox.h"
#include "GUI/Layout/Units.h"
#include "GUI/Slider.h"
#include "GUI/Widget.h"
#include "GUI/WidgetContainer.h"
#include "GUI/WidgetView.h"
#include "Math/Transform.h"
#include "Math/Vec.h"

namespace gui
{

class Scrollbar : public WidgetContainer,
                  private ButtonController,
                  private SliderController
{
  using Base = WidgetContainer;

public:
  Scrollbar(layout::Length width, Widget* widget,
            const sf::Texture& button_texture);

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

#endif /* Scrollbar.h */
