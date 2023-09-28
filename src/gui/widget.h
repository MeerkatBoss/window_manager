/**
 * @file widget.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 * 
 * @brief
 *
 * @version 0.1
 * @date 2023-09-15
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __WIDGETS_WIDGET_H
#define __WIDGETS_WIDGET_H

#include <cstddef>

#include <SFML/Graphics.hpp>

#include "event/event.h"
#include "event/event_handler.h"

#include "event/keys.h"
#include "math/transform.h"
#include "math/vec.h"

namespace gui
{

class Widget : public event::EventHandler
{
public:
  Widget(const math::Transform& transform = math::Transform()) :
    m_transform(transform),
    m_id(++s_idCounter)
  {
  }


  const math::Transform& transform() const { return m_transform; }
        math::Transform& transform()       { return m_transform; }

  size_t getId() const { return m_id; }

  virtual void draw(sf::RenderTarget& draw_target,
              const math::Transform&  parent_transform = math::Transform()) = 0;

  virtual ~Widget() = default;

protected:
  virtual bool containsPoint(
                  const math::Point& point,
                  const math::Transform&  parent_transform = math::Transform());
                          
private:
  static size_t s_idCounter;

  math::Transform m_transform;
  const size_t    m_id;
};

} // namespace ui


#endif /* widget.h */
