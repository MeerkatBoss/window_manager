/**
 * @file layout_box.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-10-27
 *
 * @copyright Copyright MeerkatBoss (c) 2023 */
#ifndef __GUI_LAYOUT_LAYOUT_BOX_H
#define __GUI_LAYOUT_LAYOUT_BOX_H

#include "Math/Vec.h"

namespace gui
{

namespace layout
{

class LayoutBox
{
public:
  virtual void updateParent(const LayoutBox& parent_box) = 0;

  virtual math::Vec   getSize() const        = 0;
  virtual math::Point getPosition() const    = 0;
  virtual math::Point getLocalOrigin() const = 0;

  virtual bool setSize(const math::Vec& size)              = 0;
  virtual bool setPosition(const math::Point& position)    = 0;
  virtual bool setLocalOrigin(const math::Point& position) = 0;

  virtual LayoutBox* copy(void) const = 0;

  virtual ~LayoutBox() = default;
};

inline math::Point getAbsoluteOrigin(const LayoutBox* box)
{
  return math::Point(box->getSize().x * box->getLocalOrigin().x,
                     box->getSize().y * box->getLocalOrigin().y);
}

struct Rect
{
  math::Point tl, tr, bl, br;
};

inline Rect getRect(const math::Vec& size)
{
  return {.tl = math::Point(0, 0),
          .tr = math::Point(size.x, 0),
          .bl = math::Point(0, size.y),
          .br = math::Point(size.x, size.y)};
}

} // namespace layout

} // namespace gui

#endif /* LayoutBox.h */
