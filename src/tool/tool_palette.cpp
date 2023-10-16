#include "tool/tool_palette.h"
#include "tool/brush_tool.h"
#include "tool/ellipse_tool.h"
#include "tool/line_tool.h"
#include "tool/polyline_tool.h"
#include "tool/rectangle_tool.h"

namespace tool
{

ToolPalette::ToolPalette() :
  m_brush(new BrushTool(5, *this)),
  m_line(new LineTool(*this)),
  m_rectangle(new RectangleTool(*this)),
  m_ellipse(new EllipseTool(*this)),
  m_polyline(new PolylineTool(*this)),
  m_fill(new FillTool(*this)),
  m_fgColor(sf::Color::Black),
  m_bgColor(sf::Color::White),
  m_activeTool(m_brush)
{

}

ToolPalette::~ToolPalette()
{
  delete m_brush;
  delete m_line;
  delete m_rectangle;
  delete m_ellipse;
  delete m_polyline;
  delete m_fill;
}

} // namespace tool
