/**
 * @file event_handler.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-09-28
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __EVENT_EVENT_HANDLER_H
#define __EVENT_EVENT_HANDLER_H

#include "Event/Event.h"
#include "Math/TransformStack.h"

namespace event
{

class EventHandler
{
public:
  virtual bool onEvent(const Event& event);

  virtual bool onMousePressed(MouseKey) { return false; }
  virtual bool onMouseReleased(MouseKey) { return false; }

  virtual bool onMouseMoved(const math::Vec&, math::TransformStack&)
  {
    return false;
  }

  virtual bool onKeyboardPressed(KeyboardKey) { return false; }
  virtual bool onKeyboardReleased(KeyboardKey) { return false; }

  virtual bool onUpdate(double) { return false; }

  virtual ~EventHandler() = default;
};

} // namespace event

#endif /* EventHandler.h */
