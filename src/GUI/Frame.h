/**
 * @file frame.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-10-03
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_FRAME_H
#define __GUI_FRAME_H

#include <SFML/Graphics/Texture.hpp>

#include "Event/Event.h"
#include "GUI/Button.h"
#include "GUI/Layout/DefaultBox.h"
#include "GUI/Widget.h"
#include "GUI/WidgetContainer.h"
#include "Math/Transform.h"
#include "Math/Vec.h"
namespace gui
{

class Frame : public WidgetContainer, private ButtonController
{
  using Base = WidgetContainer;

public:
  Frame(const layout::Length& width, Widget* widget,
        const sf::Texture& button_texture);

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

  virtual void onClick(size_t) override { m_resizing = true; }
  virtual void onRelease(size_t) override { m_resizing = false; }

private:
  bool      m_moving;
  bool      m_resizing;
  math::Vec m_lastPos;
};

} // namespace gui

#endif /* Frame.h */
