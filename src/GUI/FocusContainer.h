/**
 * @file FocusContainer.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-07
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_FOCUS_CONTAINER_H
#define __GUI_FOCUS_CONTAINER_H

#include "GUI/Layout/LayoutBox.h"
#include "GUI/WidgetContainer.h"
namespace gui
{

class FocusContainer : public WidgetContainer
{
public:
  FocusContainer(const layout::LayoutBox& layout_box) :
      WidgetContainer(layout_box), m_focused(nullptr)
  {
  }

  virtual bool onEvent(const event::Event& event) override;

  virtual void draw(sf::RenderTarget&     draw_target,
                    math::TransformStack& transform_stack) override;

private:
  Widget* m_focused;
};

} // namespace gui

#endif /* FocusContainer.h */
