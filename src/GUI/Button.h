/**
 * @file button.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-09-21
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_BUTTON_H
#define __GUI_BUTTON_H

#include <SFML/Graphics/Texture.hpp>
#include <cstddef>

#include "GUI/Layout/LayoutBox.h"
#include "GUI/Widget.h"
#include "Math/Transform.h"
#include "Math/Vec.h"

namespace gui
{

class ButtonController
{
public:
  virtual void onClick([[maybe_unused]] size_t button_id) { return; }
  virtual void onHold([[maybe_unused]] size_t button_id,
                      [[maybe_unused]] double delta_time)
  {
    return;
  }
  virtual void onRelease([[maybe_unused]] size_t button_id) { return; }

  virtual ~ButtonController() = default;
};

class Button : public Widget
{
public:
  Button(ButtonController& controller, const sf::Texture& texture,
         layout::LayoutBox* layout_box) :
      Widget(layout_box),
      m_texture(texture),
      m_textureTransform(
          math::Vec(-0.5, -0.5),
          math::Vec(1.0 / texture.getSize().x, 1.0 / texture.getSize().y)),
      m_controller(controller),
      m_pressed(false),
      m_hovered(false)
  {
  }

  virtual bool onMousePressed(const math::Vec&      position,
                              event::MouseKey       mouse_button,
                              math::TransformStack& transform_stack) override;

  virtual bool onMouseReleased(const math::Vec&      position,
                               event::MouseKey       mouse_button,
                               math::TransformStack& transform_stack) override;

  virtual bool onMouseMoved(const math::Vec&      position,
                            math::TransformStack& transform_stack) override;

  virtual bool onTick(double delta_time) override;

  virtual void draw(sf::RenderTarget&     draw_target,
                    math::TransformStack& transform_stack) override;

private:
  const sf::Texture& m_texture;
  math::Transform    m_textureTransform;

  ButtonController& m_controller;
  bool              m_pressed;
  bool              m_hovered;
};

} // namespace gui

#endif /* Button.h */
