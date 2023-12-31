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
#ifndef __GUI_WIDGET_H
#define __GUI_WIDGET_H

#include <SFML/Graphics.hpp>
#include <cstddef>

#include "Event/EventHandler.h"
#include "GUI/Layout/LayoutBox.h"
#include "Math/Transform.h"
#include "Math/TransformStack.h"
#include "Math/Vec.h"

namespace gui
{

class Widget : public event::EventHandler
{
public:
  Widget(const layout::LayoutBox& layout_box) :
      m_layoutBox(layout_box.clone()), m_id(++s_idCounter)
  {
  }

  Widget& operator=(const Widget&) = delete;

  virtual ~Widget() { delete m_layoutBox; }

  void setLayoutBox(const layout::LayoutBox& layout_box)
  {
    delete m_layoutBox;
    m_layoutBox = layout_box.clone();
  }

  layout::LayoutBox&       getLayoutBox(void) { return *m_layoutBox; }
  const layout::LayoutBox& getLayoutBox(void) const { return *m_layoutBox; }

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

#endif /* Widget.h */
