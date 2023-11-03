#include "event/event_emitter.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "event/event.h"
#include "event/keys.h"

namespace event
{

static inline MouseKey getMouseKey(sf::Mouse::Button sf_button)
{
  using Button = sf::Mouse::Button;

  switch (sf_button)
  {
  case Button::Left:
    return MouseKey::Left;
  case Button::Right:
    return MouseKey::Right;
  case Button::Middle:
    return MouseKey::Left;

  case Button::XButton1:
  case Button::XButton2:
  case Button::ButtonCount:
  default:
    return MouseKey::Extra;
  }
}

static KeyboardKey getKeyboardKey(sf::Keyboard::Key key);

Event* EventEmitter::emitEvent(const sf::Event& sf_event)
{
  /* TODO: overload operator new for events to avoid excessive allocations */

  if (sf_event.type == sf::Event::MouseButtonPressed ||
      sf_event.type == sf::Event::MouseButtonReleased)
  {
    const sf::Event::MouseButtonEvent& sf_mouse_event = sf_event.mouseButton;

    MouseKey key      = getMouseKey(sf_mouse_event.button);
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

  if (sf_event.type == sf::Event::KeyPressed ||
      sf_event.type == sf::Event::KeyReleased)
  {
    const sf::Event::KeyEvent& sf_key_event = sf_event.key;

    KeyboardKey key      = getKeyboardKey(sf_key_event.code);
    KeyState    keyState = sf_event.type == sf::Event::KeyPressed
                            ? KeyState::Pressed
                            : KeyState::Released;
    return new KeyboardEvent(keyState, key);
  }

  return nullptr;
}

static KeyboardKey getKeyboardKey(sf::Keyboard::Key key)
{
#define CONVERT_KEY(key)                                                       \
  case sf::Keyboard::key:                                                      \
    return KeyboardKey::key
  switch (key)
  {
    CONVERT_KEY(A);
    CONVERT_KEY(B);
    CONVERT_KEY(C);
    CONVERT_KEY(D);
    CONVERT_KEY(E);
    CONVERT_KEY(F);
    CONVERT_KEY(G);
    CONVERT_KEY(H);
    CONVERT_KEY(I);
    CONVERT_KEY(J);
    CONVERT_KEY(K);
    CONVERT_KEY(L);
    CONVERT_KEY(M);
    CONVERT_KEY(N);
    CONVERT_KEY(O);
    CONVERT_KEY(P);
    CONVERT_KEY(Q);
    CONVERT_KEY(R);
    CONVERT_KEY(S);
    CONVERT_KEY(T);
    CONVERT_KEY(U);
    CONVERT_KEY(V);
    CONVERT_KEY(W);
    CONVERT_KEY(X);
    CONVERT_KEY(Y);
    CONVERT_KEY(Z);
    CONVERT_KEY(Num0);
    CONVERT_KEY(Num1);
    CONVERT_KEY(Num2);
    CONVERT_KEY(Num3);
    CONVERT_KEY(Num4);
    CONVERT_KEY(Num5);
    CONVERT_KEY(Num6);
    CONVERT_KEY(Num7);
    CONVERT_KEY(Num8);
    CONVERT_KEY(Num9);
    CONVERT_KEY(Escape);
    CONVERT_KEY(LControl);
    CONVERT_KEY(LShift);
    CONVERT_KEY(LAlt);
    CONVERT_KEY(LSystem);
    CONVERT_KEY(RControl);
    CONVERT_KEY(RShift);
    CONVERT_KEY(RAlt);
    CONVERT_KEY(RSystem);
    CONVERT_KEY(Menu);
    CONVERT_KEY(LBracket);
    CONVERT_KEY(RBracket);
    CONVERT_KEY(Semicolon);
    CONVERT_KEY(Comma);
    CONVERT_KEY(Period);
    CONVERT_KEY(Quote);
    CONVERT_KEY(Slash);
    CONVERT_KEY(Backslash);
    CONVERT_KEY(Tilde);
    CONVERT_KEY(Equal);
    CONVERT_KEY(Hyphen);
    CONVERT_KEY(Space);
    CONVERT_KEY(Enter);
    CONVERT_KEY(Backspace);
    CONVERT_KEY(Tab);
    CONVERT_KEY(PageUp);
    CONVERT_KEY(PageDown);
    CONVERT_KEY(End);
    CONVERT_KEY(Home);
    CONVERT_KEY(Insert);
    CONVERT_KEY(Delete);
    CONVERT_KEY(Add);
    CONVERT_KEY(Subtract);
    CONVERT_KEY(Multiply);
    CONVERT_KEY(Divide);
    CONVERT_KEY(Left);
    CONVERT_KEY(Right);
    CONVERT_KEY(Up);
    CONVERT_KEY(Down);
    CONVERT_KEY(Numpad0);
    CONVERT_KEY(Numpad1);
    CONVERT_KEY(Numpad2);
    CONVERT_KEY(Numpad3);
    CONVERT_KEY(Numpad4);
    CONVERT_KEY(Numpad5);
    CONVERT_KEY(Numpad6);
    CONVERT_KEY(Numpad7);
    CONVERT_KEY(Numpad8);
    CONVERT_KEY(Numpad9);
    CONVERT_KEY(F1);
    CONVERT_KEY(F2);
    CONVERT_KEY(F3);
    CONVERT_KEY(F4);
    CONVERT_KEY(F5);
    CONVERT_KEY(F6);
    CONVERT_KEY(F7);
    CONVERT_KEY(F8);
    CONVERT_KEY(F9);
    CONVERT_KEY(F10);
    CONVERT_KEY(F11);
    CONVERT_KEY(F12);
    CONVERT_KEY(F13);
    CONVERT_KEY(F14);
    CONVERT_KEY(F15);
    CONVERT_KEY(Pause);

    CONVERT_KEY(KeyCount);

  case sf::Keyboard::Unknown:
  default:
    return KeyboardKey::KeyCount;
  }
#undef CONVERT_KEY
}

} // namespace event
