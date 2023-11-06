#include "Event/EventHandler.h"

#include "Event/Event.h"
#include "Math/TransformStack.h"

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
      return onMousePressed(mouse_event.position,
                            mouse_event.button,
                            mouse_event.transform_stack);
    else
      return onMouseReleased(mouse_event.position,
                             mouse_event.button,
                             mouse_event.transform_stack);
  }

  if (event_type == MouseMove)
  {
    const MouseMoveEvent& mouse_event =
        static_cast<const MouseMoveEvent&>(event);

    return onMouseMoved(mouse_event.position,
                        mouse_event.transform_stack);
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

    return onTick(update_event.deltaTimeSec);
  }

  return false;
}

} // namespace event
