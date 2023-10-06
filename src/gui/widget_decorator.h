/**
 * @file widget_decorator.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 * 
 * @brief
 *
 * @version 0.1
 * @date 2023-10-10
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_WIDGET_DECORATOR_H
#define __GUI_WIDGET_DECORATOR_H

#include "event/event.h"
#include "event/keys.h"
#include "math/transform.h"
#include "gui/widget.h"

namespace gui
{

class WidgetDecorator : public Widget
{
public:
  WidgetDecorator(const math::Transform& transform,
                  Widget* decorated) :
    Widget(transform),
    m_decorated(decorated),
    m_focused(false)
  {
  }

  virtual ~WidgetDecorator() override
  {
    delete m_decorated;
  }

  virtual bool onEvent(const event::Event& event) override
  {
    if (m_focused && needEventForward(event))
    {
      if (m_decorated->onEvent(event))
      {
        return true;
      }
    }

    return Widget::onEvent(event);
  }

  virtual bool onMouseMoved(const math::Vec& position,
                            math::TransformStack& transform_stack) override
  {
    transform_stack.enterCoordSystem(transform());
    math::Point local_position = transform_stack.getCoordSystem()
                                                .restorePoint(position);

    m_focused =
      -0.5 < local_position.x && local_position.x < 0.5 &&
      -0.5 < local_position.y && local_position.y < 0.5;

    bool handled = m_decorated->onMouseMoved(position, transform_stack);

    transform_stack.exitCoordSystem();

    return Widget::onMouseMoved(position, transform_stack) || handled;
  }

  virtual bool onMouseReleased(event::MouseKey mouse_button) override
  {
    bool handled = m_decorated->onMouseReleased(mouse_button);

    return Widget::onMouseReleased(mouse_button) || handled;
  }

  virtual bool onUpdate(double delta_time) override
  {
    bool handled = m_decorated->onUpdate(delta_time);

    return Widget::onUpdate(delta_time) || handled;
  }
protected:
  bool isFocused() const { return m_focused; }
  Widget* getDecorated() { return m_decorated; }
  const Widget* getDecorated() const
  {
    return const_cast<WidgetDecorator*>(this)->getDecorated();
  }

  bool needEventForward(const event::Event& event) const
  {
    size_t type = event.getEventType();
    if (type == event::EventType::MouseMove ||
        type == event::EventType::Update)
    {
      return false;
    }
    if (type != event::EventType::MouseButton)
    {
      return true;
    }

    event::KeyState key_state =
      static_cast<const event::MouseButtonEvent&>(event).buttonState;

    return key_state != event::KeyState::Released;
  }

private:

  Widget* m_decorated;
  bool    m_focused;
};

} // namespace gui

#endif /* widget_decorator.h */
