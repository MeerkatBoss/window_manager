/**
 * @file tool.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-10-16
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __TOOL_TOOL_H
#define __TOOL_TOOL_H

#include <gui/canvas.h>
#include <gui/widget.h>
#include <math/vec.h>

namespace tool
{

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

enum class ButtonState
{
  Pressed,
  Released
};

class Tool
{
public:
  virtual void onMainButton(ButtonState state, const math::Vec& pos,
                            gui::Canvas& canvas)
  {
    return;
  }

  virtual void onSecondaryButton(ButtonState state, const math::Vec& pos,
                                 gui::Canvas& canvas)
  {
    return;
  }

  virtual void onModifier1(ButtonState state, const math::Vec& pos,
                           gui::Canvas& canvas)
  {
    return;
  }

  virtual void onModifier2(ButtonState state, const math::Vec& pos,
                           gui::Canvas& canvas)
  {
    return;
  }

  virtual void onModifier3(ButtonState state, const math::Vec& pos,
                           gui::Canvas& canvas)
  {
    return;
  }

  virtual void onMove(const math::Vec& pos, gui::Canvas& canvas) { return; }

  virtual void onConfirm(const math::Vec& pos, gui::Canvas& canvas) { return; }

  virtual void onCancel(const math::Vec& pos, gui::Canvas& canvas) { return; }

  virtual gui::Widget* getWidget() { return nullptr; }

  virtual ~Tool() = default;
};

} // namespace tool

#pragma clang diagnostic pop

#endif /* tool.h */
