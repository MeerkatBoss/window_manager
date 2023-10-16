/**
 * @file line_tool.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 * 
 * @brief
 *
 * @version 0.1
 * @date 2023-10-16
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __TOOL_LINE_TOOL_H
#define __TOOL_LINE_TOOL_H

#include "tool/tool.h"

namespace tool
{

class ToolPalette;

class LineTool : public Tool
{
public:
  LineTool(const ToolPalette& palette);

  virtual void onMainButton(ButtonState state,
                            const math::Vec& pos,
                            gui::Canvas& canvas) override;

  virtual void onModifier1(ButtonState state,
                           const math::Vec& pos,
                           gui::Canvas& canvas) override;

  virtual void onMove(const math::Vec& pos, gui::Canvas& canvas) override;

  virtual void onConfirm(const math::Vec& pos, gui::Canvas& canvas) override;

  virtual void onCancel(const math::Vec& pos, gui::Canvas& canvas) override;

  virtual gui::Widget* getWidget() override
  {
    return m_active ? m_preview : nullptr;
  }

  virtual ~LineTool() override
  {
    delete m_preview;
  }

private:
  void updateEndPos(const math::Vec& pos);

  bool         m_active;
  gui::Widget* m_preview;
  bool         m_aligned;
  math::Vec    m_startPos;
  math::Vec    m_endPos;

  const ToolPalette& m_palette;
};

} // namespace tool

#endif /* line_tool.h */
