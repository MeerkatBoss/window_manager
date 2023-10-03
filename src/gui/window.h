/**
 * @file window.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 * 
 * @brief
 *
 * @version 0.1
 * @date 2023-10-03
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_WINDOW_H
#define __GUI_WINDOW_H

#include "gui/widget.h"
#include "math/vec.h"

namespace gui
{

class Window : public Widget
{
public:
  Window(const math::Point& position, const math::Vec& scale) :
    Widget(math::Transform(position, scale))
  {

  }

  virtual void draw(sf::RenderTarget& draw_target,
                    math::TransformStack& transform_stack) override;
};

} // namespace gui

#endif /* window.h */
