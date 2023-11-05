/**
 * @file keys.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-09-28
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __EVENT_KEYS_H
#define __EVENT_KEYS_H

namespace event
{

enum class KeyState
{
  Pressed,
  Released
};

enum class MouseKey
{
  Left,
  Middle,
  Right,
  Extra
};

enum class KeyboardKey
{
  A = 0,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,
  Num0,
  Num1,
  Num2,
  Num3,
  Num4,
  Num5,
  Num6,
  Num7,
  Num8,
  Num9,
  Escape,
  LControl,
  LShift,
  LAlt,
  LSystem,
  RControl,
  RShift,
  RAlt,
  RSystem,
  Menu,
  LBracket,
  RBracket,
  Semicolon,
  Comma,
  Period,
  Quote,
  Slash,
  Backslash,
  Tilde,
  Equal,
  Hyphen,
  Space,
  Enter,
  Backspace,
  Tab,
  PageUp,
  PageDown,
  End,
  Home,
  Insert,
  Delete,
  Add,
  Subtract,
  Multiply,
  Divide,
  Left,
  Right,
  Up,
  Down,
  Numpad0,
  Numpad1,
  Numpad2,
  Numpad3,
  Numpad4,
  Numpad5,
  Numpad6,
  Numpad7,
  Numpad8,
  Numpad9,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
  F13,
  F14,
  F15,
  Pause,

  KeyCount,
};

} // namespace event

#endif /* Keys.h */
