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
#include "filter/filter_mask.h"
#include "gui/widget.h"
#include "math/transform.h"
#include "math/vec.h"

namespace tool
{
  class ToolPalette;
}

namespace filter
{
  class FilterPalette;
}

namespace gui
{

class Canvas : public Widget
{
public:
  Canvas(tool::ToolPalette& palette, filter::FilterPalette& filters,
         size_t width_px, size_t height_px,
         const math::Point& position = math::Point()) :
    Canvas(palette, filters, width_px, height_px, position,
           math::Vec(double(width_px) / double(height_px), 1))
  {
  }

  Canvas(tool::ToolPalette& palette, filter::FilterPalette& filters,
         size_t width_px, size_t height_px,
         const math::Point& position,
         const math::Vec&   scale) :
    Widget(math::Transform(position, scale)),
    m_renderTexture(),
    m_textureTransform(math::Vec(-0.5, -0.5),
                       math::Vec(1.0/width_px, 1.0/height_px)),
    m_palette(palette),
    m_filters(filters),
    m_mask(width_px, height_px),
    m_hovered(false),
    m_lastPos()
  {
    m_renderTexture.create(width_px, height_px);
    m_renderTexture.clear(sf::Color::White);
    m_mask.fill(true);
  }

  virtual bool onMousePressed(event::MouseKey mouse_button) override;
  virtual bool onMouseReleased(event::MouseKey mouse_button) override;

  virtual bool onMouseMoved(const math::Vec& position,
                            math::TransformStack& transform_stack) override;

  virtual bool onKeyboardPressed(event::KeyboardKey key) override;
  virtual bool onKeyboardReleased(event::KeyboardKey key) override;

  virtual void draw(sf::RenderTarget& draw_target,
                    math::TransformStack& transform_stack) override;

  sf::RenderTexture& getRenderTexture() { return m_renderTexture; }

private:
  sf::RenderTexture      m_renderTexture;// TODO: Extract to Document
  math::Transform        m_textureTransform;
  tool::ToolPalette&     m_palette;
  filter::FilterPalette& m_filters;
  filter::FilterMask     m_mask;

  bool              m_hovered;
  bool              m_control;
  math::Point       m_lastPos;
};

} // namespace gui

#endif /* canvas.h */
