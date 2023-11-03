#include "tool/brush_tool.h"

#include <SFML/Graphics/CircleShape.hpp>

#include "tool/tool_palette.h"

namespace tool
{

void BrushTool::onMainButton(ButtonState state, const math::Vec& pos,
                             gui::Canvas& canvas)
{
  m_drawing = (state == ButtonState::Pressed);
  onMove(pos, canvas);
}

void BrushTool::onMove(const math::Vec& pos, gui::Canvas& canvas)
{
  if (!m_drawing)
    return;

  sf::CircleShape circle(m_penSize / 2);
  circle.setOrigin(m_penSize / 2, m_penSize / 2);
  circle.setPosition(pos);
  circle.setFillColor(m_palette.getForegroundColor());

  canvas.getRenderTexture().draw(circle);
}

} // namespace tool
