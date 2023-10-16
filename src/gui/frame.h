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
#include "event/event.h"
#include "gui/button.h"
#include "gui/widget.h"
#include "gui/widget_container.h"
#include "math/transform.h"
#include "math/vec.h"
namespace gui
{

class Frame : public WidgetContainer, private ButtonController
{
using Base = WidgetContainer;
public:
  Frame(double width, Widget* widget, const sf::Texture& button_texture) :
    WidgetContainer(math::Transform(
          widget->transform().getPosition(),
          widget->transform().getScale() + 2*math::Vec(width, width))),
    m_moving(false),
    m_resizing(false),
    m_lastPos()
  {
    const math::Vec& orig_scale = widget->transform().getScale();
    const math::Vec local_scale(
        orig_scale.x / (orig_scale.x + 2*width),
        orig_scale.y / (orig_scale.y + 2*width));
    widget->transform() = math::Transform(math::Point(), local_scale);

    addWidget(widget);;
    addWidget(new Button(*this, button_texture, 
                   math::Point(.5 - width/2, .5-width/2),
                   math::Vec(width, width)));
  }

  virtual bool onMousePressed(event::MouseKey mouse_button) override;
  virtual bool onMouseReleased(event::MouseKey mouse_button) override;

  virtual bool onMouseMoved(const math::Vec& position,
                            math::TransformStack& transform_stack) override;

  virtual void draw(sf::RenderTarget& draw_target,
                    math::TransformStack& transform_stack) override;

  virtual void onClick(size_t) override { m_resizing = true; }
  virtual void onRelease(size_t) override { m_resizing = false; }
  
private:
  bool      m_moving;
  bool      m_resizing;
  math::Vec m_lastPos;
};

} // namespace gui

#endif /* frame.h */
