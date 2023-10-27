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
#ifndef __GUI_WIDGET_CONTAINER_H
#define __GUI_WIDGET_CONTAINER_H

#include "event/event.h"
#include "event/keys.h"
#include "gui/widget.h"
#include "math/transform.h"
#include "util/dyn_array.h"

namespace gui
{

class WidgetContainer : public Widget
{
public:
  WidgetContainer(layout::LayoutBox* layout_box) :
      Widget(layout_box), m_widgets(), m_focused(false)
  {
  }

  virtual ~WidgetContainer() override
  {
    for (size_t i = 0; i < m_widgets.getSize(); ++i)
    {
      delete m_widgets[i];
    }
  }

  virtual bool onEvent(const event::Event& event) override
  {
    if (m_focused && needEventForward(event))
    {
      for (size_t i = 0; i < m_widgets.getSize(); ++i)
      {
        if (m_widgets[i]->onEvent(event))
        {
          return true;
        }
      }
    }

    return Widget::onEvent(event);
  }

  virtual bool onMouseMoved(const math::Vec&      position,
                            math::TransformStack& transform_stack) override
  {
    m_focused = containsPoint(position, transform_stack);

    transform_stack.enterCoordSystem(getLocalTransform());

    bool handled = false;
    for (size_t i = 0; i < m_widgets.getSize(); ++i)
    {
      handled |= m_widgets[i]->onMouseMoved(position, transform_stack);
    }

    transform_stack.exitCoordSystem();

    return Widget::onMouseMoved(position, transform_stack) || handled;
  }

  virtual bool onMouseReleased(event::MouseKey mouse_button) override
  {
    bool handled = false;

    for (size_t i = 0; i < m_widgets.getSize(); ++i)
    {
      handled |= m_widgets[i]->onMouseReleased(mouse_button);
    }

    return Widget::onMouseReleased(mouse_button) || handled;
  }

  virtual bool onUpdate(double delta_time) override
  {
    bool handled = false;
    for (size_t i = 0; i < m_widgets.getSize(); ++i)
    {
      handled |= m_widgets[i]->onUpdate(delta_time);
    }

    return Widget::onUpdate(delta_time) || handled;
  }

  void draw(sf::RenderTarget&     draw_target,
            math::TransformStack& transform_stack) override
  {
    if (m_widgets.isEmpty())
    {
      return;
    }

    transform_stack.enterCoordSystem(getLocalTransform());
    for (size_t i = m_widgets.getSize(); i > 0; --i)
    {
      m_widgets[i - 1]->draw(draw_target, transform_stack);
    }
    transform_stack.exitCoordSystem();
  }

  virtual void onLayoutUpdate(const layout::LayoutBox& parent_box) override
  {
    getLayoutBox()->updateParent(parent_box);
    size_t widget_count = m_widgets.getSize();
    for (size_t i = 0; i < widget_count; ++i)
    {
      m_widgets[i]->onLayoutUpdate(*getLayoutBox());
    }
  }

protected:
  bool isFocused(void) const { return m_focused; }

  void focus(void) { m_focused = true; }
  void unfocus(void) { m_focused = false; }

  util::DynArray<Widget*>&       getWidgets() { return m_widgets; }
  const util::DynArray<Widget*>& getWidgets() const
  {
    return const_cast<WidgetContainer*>(this)->getWidgets();
  }

  void addWidget(Widget* widget) { m_widgets.pushBack(widget); }

  bool needEventForward(const event::Event& event) const
  {
    size_t type = event.getEventType();
    if (type == event::EventType::MouseMove || type == event::EventType::Update)
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
  util::DynArray<Widget*> m_widgets;
  bool                    m_focused;
};

} // namespace gui

#endif /* widget_decorator.h */
