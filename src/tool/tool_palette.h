/**
 * @file tool_palette.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-10-17
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __TOOL_TOOL_PALETTE_H
#define __TOOL_TOOL_PALETTE_H

#include "tool/brush_tool.h"
#include "tool/ellipse_tool.h"
#include "tool/fill_tool.h"
#include "tool/line_tool.h"
#include "tool/polyline_tool.h"
#include "tool/rectangle_tool.h"
#include "tool/tool.h"

namespace tool
{

class ToolPalette
{
public:
  ToolPalette();
  ~ToolPalette();

  Tool* getActiveTool() { return m_activeTool; }

  const sf::Color& getForegroundColor() const { return m_fgColor; }

  const sf::Color& getBackgroundColor() const { return m_bgColor; }

  void selectBrushTool()     { m_activeTool = m_brush; }
  void selectLineTool()      { m_activeTool = m_line; }
  void selectRectangleTool() { m_activeTool = m_rectangle; }
  void selectEllipseTool()   { m_activeTool = m_ellipse; }
  void selectPolylineTool()  { m_activeTool = m_polyline; }
  void selectFillTool()      { m_activeTool = m_fill; }

private:
  BrushTool*     m_brush;
  LineTool*      m_line;
  RectangleTool* m_rectangle;
  EllipseTool*   m_ellipse;
  PolylineTool*  m_polyline;
  FillTool*      m_fill;

  sf::Color m_fgColor;
  sf::Color m_bgColor;

  Tool* m_activeTool;
};

} // namespace tool

#endif /* tool_palette.h */
