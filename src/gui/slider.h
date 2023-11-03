/**
 * @file slider.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-10-10
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_SLIDER_H
#define __GUI_SLIDER_H

#include "gui/layout/layout_box.h"
#include "gui/widget.h"
#include "math/transform.h"
#include "math/vec.h"

namespace gui
{

class SliderController
{
public:
  virtual void      setValue(size_t slider_id, const math::Vec& val) = 0;
  virtual math::Vec getValue(size_t slider_id)                       = 0;

  virtual ~SliderController() = default;
};

class Slider : public Widget
{
public:
  Slider(SliderController& controller, layout::LayoutBox* layout_box,
         const math::Vec& handle_scale) :
      Widget(layout_box),
      m_controller(controller),
      m_handleScale(handle_scale),
      m_hovered(false),
      m_captured(false),
      m_pendingVal()
  {
  }

  virtual bool onMousePressed(event::MouseKey mouse_button) override;
  virtual bool onMouseReleased(event::MouseKey mouse_button) override;

  virtual bool onMouseMoved(const math::Vec&      position,
                            math::TransformStack& transform_stack) override;

  virtual void draw(sf::RenderTarget&     draw_target,
                    math::TransformStack& transform_stack) override;

private:
  void drawBackground(sf::RenderTarget&     draw_target,
                      math::TransformStack& transform_stack);

  void drawHandle(sf::RenderTarget&     draw_target,
                  math::TransformStack& transform_stack);

  SliderController& m_controller;
  math::Vec         m_handleScale;
  bool              m_hovered;
  bool              m_captured;
  math::Vec         m_pendingVal;
};

} // namespace gui

#endif /* slider.h */
