#include "event/event_handler.h"

#include "event/event.h"
#include "math/transform_stack.h"

namespace event
{

bool EventHandler::onEvent(const Event& event)
{
  size_t event_type = event.getEventType();

  if (event_type == MouseButton)
  {
    const MouseButtonEvent& mouse_event =
        static_cast<const MouseButtonEvent&>(event);

    if (mouse_event.buttonState == KeyState::Pressed)
      return onMousePressed(mouse_event.button);
    else
      return onMouseReleased(mouse_event.button);
  }

  if (event_type == MouseMove)
  {
    const MouseMoveEvent& mouse_event =
        static_cast<const MouseMoveEvent&>(event);

    math::TransformStack& transform_stack =
        const_cast<MouseMoveEvent&>(mouse_event).transform_stack;

    return onMouseMoved(mouse_event.position, transform_stack);
  }

  if (event_type == Keyboard)
  {
    const KeyboardEvent& keyboard_event =
        static_cast<const KeyboardEvent&>(event);

    if (keyboard_event.keyState == KeyState::Pressed)
      return onKeyboardPressed(keyboard_event.key);
    else
      return onKeyboardReleased(keyboard_event.key);
  }

  if (event_type == Update)
  {
    const UpdateEvent& update_event = static_cast<const UpdateEvent&>(event);

    return onUpdate(update_event.deltaTimeSec);
  }

  return false;
}

} // namespace event
