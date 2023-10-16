/**
 * @file fill_tool.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 * 
 * @brief
 *
 * @version 0.1
 * @date 2023-10-17
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __TOOL_FILL_TOOL_H
#define __TOOL_FILL_TOOL_H

#include "tool/tool.h"
namespace tool
{

class ToolPalette;

class FillTool : public Tool
{
public:
  FillTool(const ToolPalette& palette) : m_palette(palette) {}

  virtual void onMainButton(ButtonState state,
                            const math::Vec& pos,
                            gui::Canvas& canvas) override;

private:
  const ToolPalette& m_palette;
};

} // namespace tool

#endif /* fill_tool.h */
