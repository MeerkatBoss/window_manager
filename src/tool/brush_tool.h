/**
 * @file brush_tool.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 * 
 * @brief
 *
 * @version 0.1
 * @date 2023-10-16
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __TOOL_BRUSH_TOOL_H
#define __TOOL_BRUSH_TOOL_H

#include "tool/tool.h"

namespace tool
{

class ToolPalette;

class BrushTool : public Tool
{
public:
  BrushTool(double pen_size, const ToolPalette& palette) :
    m_penSize(pen_size), m_drawing(false), m_palette(palette) {}

  virtual void onMainButton(ButtonState state,
                            const math::Vec& pos,
                            gui::Canvas& canvas) override;

  virtual void onMove(const math::Vec& pos, gui::Canvas& canvas) override;

private:
  double m_penSize;
  bool   m_drawing;
  const ToolPalette& m_palette;
};

} // namespace tool

#endif /* brush_tool.h */
