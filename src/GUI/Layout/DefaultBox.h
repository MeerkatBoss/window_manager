/**
 * @file default_resize.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-10-25
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_LAYOUT_DEFAULT_RESIZE_H
#define __GUI_LAYOUT_DEFAULT_RESIZE_H

#include <cstddef>

#include "GUI/Layout/LayoutBox.h"
#include "GUI/Layout/Units.h"

namespace gui
{

namespace layout
{

class DefaultBox : public LayoutBox
{
public:
  DefaultBox() : DefaultBox(100_per, 100_per) {}

  DefaultBox(const Length& width, const Length& height,
             Align align = Align::Free, bool resizable = true) :
      m_align(align),
      m_posX(0_px),
      m_posY(0_px),
      m_resizable(resizable),
      m_width(width),
      m_height(height),
      m_paddingTop(0_px),
      m_paddingBottom(0_px),
      m_paddingLeft(0_px),
      m_paddingRight(0_px),
      m_parentSize(1, 1)
  {
  }

  DefaultBox(const DefaultBox& other) :
      m_align(other.m_align),
      m_posX(other.m_posX),
      m_posY(other.m_posY),
      m_resizable(other.m_resizable),
      m_width(other.m_width),
      m_height(other.m_height),
      m_paddingTop(other.m_paddingTop),
      m_paddingBottom(other.m_paddingBottom),
      m_paddingLeft(other.m_paddingLeft),
      m_paddingRight(other.m_paddingRight),
      m_parentSize(1, 1)
  {
  }

  void setPadding(const Length& padding)
  {
    setPadding(padding, padding, padding, padding);
  }

  void setPadding(const Length& top_bottom, const Length& left_right)
  {
    setPadding(top_bottom, top_bottom, left_right, left_right);
  }

  void setPadding(const Length& top, const Length& bottom, const Length& left,
                  const Length& right)
  {
    m_paddingTop    = top;
    m_paddingBottom = bottom;
    m_paddingLeft   = left;
    m_paddingRight  = right;
  }

  void setSize(const Length& width, const Length& height)
  {
    setWidth(width);
    setHeight(height);
  }

  void setWidth(const Length& width) { m_width = width; }
  void setHeight(const Length& height) { m_height = height; }

  void setAlignment(Align align) { m_align = align; }
  void unlockSize() { m_resizable = true; }
  void lockSize() { m_resizable = false; }

  virtual void updateParent(const LayoutBox& parent_box) override;

  virtual math::Vec   getSize() const override;
  virtual math::Point getPosition() const override;

  virtual bool setSize(const math::Vec& size) override;
  virtual bool setPosition(const math::Point& position) override;

  virtual LayoutBox* clone(void) const override
  {
    return new DefaultBox(*this);
  }

private:
  Align  m_align;
  Length m_posX;
  Length m_posY;

  bool   m_resizable;
  Length m_width;
  Length m_height;

  Length m_paddingTop;
  Length m_paddingBottom;
  Length m_paddingLeft;
  Length m_paddingRight;

  math::Vec m_parentSize;
};

} // namespace layout

} // namespace gui

#endif /* DefaultResize.h */
