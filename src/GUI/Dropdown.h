/**
 * @file Dropdown.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-07
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_DROPDOWN_H
#define __GUI_DROPDOWN_H

#include <SFML/Graphics/RenderTarget.hpp>

#include "GUI/Button.h"
#include "GUI/Layout/DefaultBox.h"
#include "GUI/Layout/LayoutBox.h"
#include "GUI/Layout/Units.h"
#include "GUI/Widget.h"
#include "GUI/WidgetContainer.h"
#include "Math/TransformStack.h"
namespace gui
{

class Dropdown : public Widget, private ButtonController
{
public:
  Dropdown(const char* name, const layout::LayoutBox& layout) :
      Widget(layout),
      m_mainButton(
          *this, assets::AssetManager::getButtonTexture(),
          layout::DefaultBox(100_per, 100_per, layout::Align::TopCenter), name),
      m_dropDown(
          layout::DefaultBox(100_per, 100_per, layout::Align::Free)),
      m_active(false)
  {
  }

  void addButton(ButtonController& controller, const char* name);

  virtual bool onEvent(const event::Event& event) override;

  virtual void onClick(size_t) override { m_active = !m_active; }

  virtual void draw(sf::RenderTarget&     draw_target,
                    math::TransformStack& transform_stack) override;

  virtual void onLayoutUpdate(const layout::LayoutBox& parent_box) override
  {
    Widget::onLayoutUpdate(parent_box);
    m_mainButton.onLayoutUpdate(getLayoutBox());
    m_dropDown.onLayoutUpdate(getLayoutBox());
  }

private:
  Button          m_mainButton;
  WidgetContainer m_dropDown;
  bool            m_active;
};

} // namespace gui

#endif /* Dropdown.h */
