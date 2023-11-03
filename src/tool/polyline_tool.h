/**
 * @file polyline_tool.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-10-17
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __TOOL_POLYLINE_TOOL_H
#define __TOOL_POLYLINE_TOOL_H

#include "gui/widget.h"
#include "math/vec.h"
#include "tool/tool.h"
#include "util/dyn_array.h"
namespace tool
{

class ToolPalette;

class PolylineTool : public Tool
{
public:
  PolylineTool(const ToolPalette& palette);

  virtual ~PolylineTool() override { delete m_preview; }

  virtual void onMainButton(ButtonState state, const math::Vec& pos,
                            gui::Canvas& canvas) override;

  virtual void onMove(const math::Vec& pos, gui::Canvas& canvas) override;

  virtual void onConfirm(const math::Vec& pos, gui::Canvas& canvas) override;

  virtual void onCancel(const math::Vec& pos, gui::Canvas& canvas) override;

  virtual gui::Widget* getWidget() override
  {
    return m_active ? m_preview : nullptr;
  }

private:
  util::DynArray<math::Vec> m_points;
  bool                      m_active;
  gui::Widget*              m_preview;

  const ToolPalette& m_palette;
};

} // namespace tool

#endif /* polyline_tool.h */
