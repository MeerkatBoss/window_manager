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
#ifndef __UI_BUTTON_H
#define __UI_BUTTON_H

#include <cstddef>

#include <SFML/Graphics/Texture.hpp>

#include "gui/layout/layout_box.h"
#include "math/transform.h"
#include "gui/widget.h"
#include "math/vec.h"

namespace gui
{

class ButtonController
{
public:
  virtual void onClick([[maybe_unused]] size_t button_id) { return; }
  virtual void onHold([[maybe_unused]] size_t button_id,
                      [[maybe_unused]] double delta_time) { return; }
  virtual void onRelease([[maybe_unused]] size_t button_id) { return; }

  virtual ~ButtonController() = default;
};

class Button : public Widget
{
public:
  Button(ButtonController& controller,
         const sf::Texture& texture,
         layout::LayoutBox* layout_box) :
    Widget(layout_box),
    m_texture(texture),
    m_textureTransform(math::Vec(-0.5, -0.5),
                       math::Vec(1.0/texture.getSize().x,
                                 1.0/texture.getSize().y)),
    m_controller(controller),
    m_pressed(false),
    m_hovered(false)
  {
  }

  virtual bool onMousePressed(event::MouseKey mouse_button) override;
  virtual bool onMouseReleased(event::MouseKey mouse_button) override;

  virtual bool onMouseMoved(const math::Vec& position,
                            math::TransformStack& transform_stack) override;

  virtual bool onUpdate(double delta_time) override;

  virtual void draw(sf::RenderTarget& draw_target,
                    math::TransformStack& transform_stack) override;

private:
  const sf::Texture& m_texture;
  math::Transform    m_textureTransform;

  ButtonController& m_controller;
  bool              m_pressed;
  bool              m_hovered;
};

} // namespace gui

#endif /* button.h */
