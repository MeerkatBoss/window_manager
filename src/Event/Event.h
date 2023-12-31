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

#include "Event/Keys.h"
#include "Math/TransformStack.h"
#include "Math/Vec.h"

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

class PositionalEvent
{
public:
  virtual const math::Vec&      getPosition(void) const       = 0;
  virtual math::TransformStack& getTransformStack(void) const = 0;

  virtual ~PositionalEvent() = default;
};

size_t registerEventType(void);

class Event
{
public:
  size_t getEventType() const { return m_eventType; }

  virtual const PositionalEvent* asPositionalEvent(void) const
  {
    return nullptr;
  }

  bool isPositionalEvent(void) const { return asPositionalEvent() != nullptr; }

  virtual ~Event() = default;

protected:
  Event(size_t eventType) : m_eventType(eventType) {}

private:
  const size_t m_eventType;
};

class MouseButtonEvent : public Event, public PositionalEvent
{
public:
  static const size_t TypeId = MouseButton;

  MouseButtonEvent(KeyState key_state, MouseKey mouse_key,
                   const math::Vec&      mouse_position,
                   math::TransformStack& stack) :
      Event(TypeId),
      buttonState(key_state),
      button(mouse_key),
      position(mouse_position),
      transform_stack(stack)
  {
  }

  virtual const math::Vec& getPosition(void) const override { return position; }

  virtual math::TransformStack& getTransformStack(void) const override
  {
    return transform_stack;
  }

  virtual const PositionalEvent* asPositionalEvent(void) const override
  {
    return this;
  }

  const KeyState        buttonState;
  const MouseKey        button;
  const math::Vec       position;
  math::TransformStack& transform_stack;
};

class MouseMoveEvent : public Event, public PositionalEvent
{
public:
  static const size_t TypeId = MouseMove;

  MouseMoveEvent(const math::Vec& mouse_position, math::TransformStack& stack) :
      Event(TypeId), position(mouse_position), transform_stack(stack)
  {
  }

  virtual const math::Vec& getPosition(void) const override { return position; }

  virtual math::TransformStack& getTransformStack(void) const override
  {
    return transform_stack;
  }

  virtual const PositionalEvent* asPositionalEvent(void) const override
  {
    return this;
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

#endif /* Event.h */
