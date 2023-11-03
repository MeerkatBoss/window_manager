/**
 * @file event.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-09-28
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __EVENT_EVENT_H
#define __EVENT_EVENT_H

#include <cstddef>

#include "event/keys.h"
#include "math/transform_stack.h"
#include "math/vec.h"

namespace event
{

enum EventType
{
  None = 0,

  MouseButton,
  MouseMove,
  Keyboard,
  Update,

  UserEventTypeMin = 256
};

size_t registerEventType(void);

class Event
{
public:
  size_t getEventType() const { return m_eventType; }

  virtual ~Event() = default;

protected:
  Event(size_t eventType) : m_eventType(eventType) {}

private:
  const size_t m_eventType;
};

class MouseButtonEvent : public Event
{
public:
  static const size_t TypeId = MouseButton;

  MouseButtonEvent(KeyState key_state, MouseKey mouse_key) :
      Event(TypeId), buttonState(key_state), button(mouse_key)
  {
  }

  const KeyState buttonState;
  const MouseKey button;
};

class MouseMoveEvent : public Event
{
public:
  static const size_t TypeId = MouseMove;

  MouseMoveEvent(const math::Vec& mouse_position, math::TransformStack& stack) :
      Event(TypeId), position(mouse_position), transform_stack(stack)
  {
  }

  const math::Vec       position;
  math::TransformStack& transform_stack;
};

class KeyboardEvent : public Event
{
public:
  static const size_t TypeId = Keyboard;

  KeyboardEvent(KeyState state, KeyboardKey keyboard_key) :
      Event(TypeId), keyState(state), key(keyboard_key)
  {
  }

  const KeyState    keyState;
  const KeyboardKey key;
};

class UpdateEvent : public Event
{
public:
  static const size_t TypeId = Update;

  UpdateEvent(double delta_time_seconds) :
      Event(TypeId), deltaTimeSec(delta_time_seconds)
  {
  }

  const double deltaTimeSec;
};

} // namespace event

#endif /* event.h */
