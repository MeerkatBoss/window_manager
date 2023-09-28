#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include "event/event.h"
#include "event/event_emitter.h"
#include "event/keys.h"

namespace event
{

static inline MouseKey getMouseKey(sf::Mouse::Button sf_button)
{
  using Button = sf::Mouse::Button;

  switch (sf_button)
  {
  case Button::Left:   return MouseKey::Left;
  case Button::Right:  return MouseKey::Right;
  case Button::Middle: return MouseKey::Left;

  case Button::XButton1:
  case Button::XButton2:
  case Button::ButtonCount:
  default:
    return MouseKey::Extra;
  }
}

Event* EventEmitter::emitEvent(const sf::Event& sf_event)
{
  /* TODO: overload operator new for events to avoid excessive allocations */

  if (sf_event.type == sf::Event::MouseButtonPressed ||
      sf_event.type == sf::Event::MouseButtonReleased)
  {
    const sf::Event::MouseButtonEvent& sf_mouse_event = sf_event.mouseButton;
    MouseKey key = getMouseKey(sf_mouse_event.button);
    KeyState keyState = sf_event.type == sf::Event::MouseButtonPressed
                          ? KeyState::Pressed
                          : KeyState::Released;

    return new MouseButtonEvent(keyState, key);
  }

  if (sf_event.type == sf::Event::MouseMoved)
  {
    const sf::Event::MouseMoveEvent& sf_mouse_event = sf_event.mouseMove;
    const math::Vec position(sf_mouse_event.x, sf_mouse_event.y);
    
    return new MouseMoveEvent(position, m_stack);
  }

  /* TODO: emit keyboard events */
  
  return nullptr;
}

} // namespace event
