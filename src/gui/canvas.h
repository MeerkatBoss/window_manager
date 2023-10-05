/**
 * @file canvas.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 * 
 * @brief
 *
 * @version 0.1
 * @date 2023-10-05
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_CANVAS_H
#define __GUI_CANVAS_H

#include <SFML/Graphics/RenderTexture.hpp>
#include "gui/widget.h"
#include "math/transform.h"
#include "math/vec.h"

namespace gui
{

class Canvas : public Widget
{
public:
  Canvas(double pen_size,
         size_t width_px, size_t height_px,
         const math::Point& position = math::Point()) :
    Canvas(pen_size, width_px, height_px, position,
           math::Vec(double(width_px) / double(height_px), 1))
  {
  }

  Canvas(double pen_size,
         size_t width_px, size_t height_px,
         const math::Point& position,
         const math::Vec&   scale) :
    Widget(math::Transform(position, scale)),
    m_renderTexture(),
    m_textureTransform(math::Vec(-0.5, -0.5),
                       math::Vec(1.0/width_px, 1.0/height_px)),
    m_penRadius(pen_size),
    m_hovered(false),
    m_drawing(false),
    m_lastPos()
  {
    m_renderTexture.create(width_px, height_px);
    m_renderTexture.clear(sf::Color::White);
  }


  virtual bool onMousePressed(event::MouseKey mouse_button) override;
  virtual bool onMouseReleased(event::MouseKey mouse_button) override;

  virtual bool onMouseMoved(const math::Vec& position,
                            math::TransformStack& transform_stack) override;

  virtual void draw(sf::RenderTarget& draw_target,
                    math::TransformStack& transform_stack) override;

private:
  void applyBrush(const math::Vec& position);

  sf::RenderTexture m_renderTexture;// TODO: Extract to Document
  math::Transform   m_textureTransform;
  double            m_penRadius;    // TODO: Extract to Tools

  bool              m_hovered;
  bool              m_drawing;
  math::Point       m_lastPos;
};

} // namespace gui

#endif /* canvas.h */
