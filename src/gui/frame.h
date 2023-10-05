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

#include "event/event.h"
#include "gui/widget.h"
#include "math/transform.h"
#include "math/vec.h"
namespace gui
{

class Frame : public Widget
{
public:
  Frame(double width, Widget* widget) :
    Widget(math::Transform(
          widget->transform().getPosition(),
          widget->transform().getScale() + 2*math::Vec(width, width))),
    m_widget(widget),
    m_captured(false),
    m_lastPos()
  {
    const math::Vec& orig_scale = widget->transform().getScale();
    const math::Vec local_scale(
        orig_scale.x / (orig_scale.x + 2*width),
        orig_scale.y / (orig_scale.y + 2*width));
    m_widget->transform() = math::Transform(math::Point(), local_scale);
  }

  virtual ~Frame() override { delete m_widget; }

  virtual bool onEvent(const event::Event& event) override
  {
    // Additional logic required here
    if (event.getEventType() == event::EventType::MouseMove)
    {
      return Widget::onEvent(event);
    }

    // Forward transparently
    bool handled = m_widget->onEvent(event);
    if (handled) return true;

    return Widget::onEvent(event);
  }

  virtual bool onMousePressed(event::MouseKey mouse_button) override;
  virtual bool onMouseReleased(event::MouseKey mouse_button) override;

  virtual bool onMouseMoved(const math::Vec& position,
                            math::TransformStack& transform_stack) override;

  virtual void draw(sf::RenderTarget& draw_target,
                    math::TransformStack& transform_stack) override;
  
private:
  Widget*   m_widget;
  bool      m_hovered;
  bool      m_captured;
  math::Vec m_lastPos;
};

} // namespace gui

#endif /* frame.h */
