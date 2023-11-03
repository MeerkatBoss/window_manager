/**
 * @file event_emitter.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-10-03
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __EVENT_EVENT_EMITTER_H
#define __EVENT_EVENT_EMITTER_H

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "event/event.h"
#include "math/transform_stack.h"

namespace event
{

class EventEmitter
{
public:
  EventEmitter(math::TransformStack& stack) : m_stack(stack) {}

  EventEmitter(const EventEmitter&) = delete;
  EventEmitter& operator=(const EventEmitter&) = delete;

  ~EventEmitter() = default;

  Event* emitEvent(const sf::Event& sf_event);

  UpdateEvent emitUpdateEvent()
  {
    return UpdateEvent(m_clock.restart().asSeconds());
  }

private:
  math::TransformStack& m_stack;
  sf::Clock             m_clock;
};

} // namespace event

#endif /* event_emitter.h */
