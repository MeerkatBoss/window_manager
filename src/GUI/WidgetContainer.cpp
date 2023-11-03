#include "GUI/WidgetContainer.h"

namespace gui
{

bool WidgetContainer::onEvent(const event::Event& event)
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

bool WidgetContainer::onMouseMoved(const math::Vec&      position,
                                   math::TransformStack& transform_stack)
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

bool WidgetContainer::onMouseReleased(event::MouseKey mouse_button)
{
  bool handled = false;

  for (size_t i = 0; i < m_widgets.getSize(); ++i)
  {
    handled |= m_widgets[i]->onMouseReleased(mouse_button);
  }

  return Widget::onMouseReleased(mouse_button) || handled;
}

bool WidgetContainer::onUpdate(double delta_time)
{
  bool handled = false;
  for (size_t i = 0; i < m_widgets.getSize(); ++i)
  {
    handled |= m_widgets[i]->onUpdate(delta_time);
  }

  return Widget::onUpdate(delta_time) || handled;
}

void WidgetContainer::draw(sf::RenderTarget&     draw_target,
                           math::TransformStack& transform_stack)
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

void WidgetContainer::onLayoutUpdate(const layout::LayoutBox& parent_box)
{
  getLayoutBox()->updateParent(parent_box);
  size_t widget_count = m_widgets.getSize();
  for (size_t i = 0; i < widget_count; ++i)
  {
    m_widgets[i]->onLayoutUpdate(*getLayoutBox());
  }
}

bool WidgetContainer::needEventForward(const event::Event& event) const
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

} // namespace gui
