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

#include <SFML/Graphics.hpp>
#include <cstddef>

#include "event/event_handler.h"
#include "gui/layout/layout_box.h"
#include "math/transform.h"
#include "math/transform_stack.h"
#include "math/vec.h"

namespace gui
{

class Widget : public event::EventHandler
{
public:
  Widget(layout::LayoutBox* layout_box) :
      m_layoutBox(layout_box), m_id(++s_idCounter)
  {
  }

  Widget& operator=(const Widget&) = delete;

  virtual ~Widget() { delete m_layoutBox; }

  void setLayoutBox(layout::LayoutBox* layout_box)
  {
    delete m_layoutBox;
    m_layoutBox = layout_box;
  }

  layout::LayoutBox*       getLayoutBox(void) { return m_layoutBox; }
  const layout::LayoutBox* getLayoutBox(void) const { return m_layoutBox; }

  math::Vec   getSize() const { return m_layoutBox->getSize(); }
  math::Point getPosition() const { return m_layoutBox->getPosition(); }

  size_t getId() const { return m_id; }

  virtual void draw(sf::RenderTarget&     draw_target,
                    math::TransformStack& transform_stack) = 0;

  virtual math::Transform getLocalTransform(void) const;

  virtual bool containsPoint(const math::Point&    point,
                             math::TransformStack& transform_stack) const;

  virtual void onLayoutUpdate(const layout::LayoutBox& parent_box);

private:
  static size_t s_idCounter;

  layout::LayoutBox* m_layoutBox;
  const size_t       m_id;
};

} // namespace gui

#endif /* widget.h */
