#include "Tool/ToolPalette.h"

#include "Tool/BrushTool.h"
#include "Tool/EllipseTool.h"
#include "Tool/LineTool.h"
#include "Tool/PolylineTool.h"
#include "Tool/RectangleTool.h"
#include "Tool/TextTool.h"

namespace tool
{

ToolPalette::ToolPalette() :
    m_brush(new BrushTool(5, *this)),
    m_line(new LineTool(*this)),
    m_rectangle(new RectangleTool(*this)),
    m_ellipse(new EllipseTool(*this)),
    m_polyline(new PolylineTool(*this)),
    m_fill(new FillTool(*this)),
    m_text(new TextTool(50, *this)),
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
