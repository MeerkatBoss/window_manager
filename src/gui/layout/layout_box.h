/**
 * @file layout_box.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-10-27
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_LAYOUT_LAYOUT_BOX_H
#define __GUI_LAYOUT_LAYOUT_BOX_H

#include "math/vec.h"

namespace gui
{

namespace layout
{

class LayoutBox
{
public:
  virtual void updateParent(const LayoutBox& parent_box) = 0;

  virtual math::Vec getSize() const = 0;
  virtual math::Point getPosition() const = 0;
  virtual math::Point getLocalOrigin() const = 0;

  virtual bool setSize(const math::Vec& size) = 0;
  virtual bool setPosition(const math::Point& position) = 0;
  virtual bool setLocalOrigin(const math::Point& position) = 0;

  virtual LayoutBox* copy(void) const = 0;

  virtual ~LayoutBox() = default;
};

} // namespace layout

} // namespace gui

#endif /* layout_box.h */
