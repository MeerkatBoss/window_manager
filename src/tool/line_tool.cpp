#include "tool/line_tool.h"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "gui/layout/default_box.h"
#include "gui/widget.h"
#include "math/transform.h"
#include "math/transform_stack.h"
#include "math/vec.h"
#include "tool/tool.h"
#include "tool/tool_palette.h"

namespace tool
{

class LinePreview : public gui::Widget
{
public:
  LinePreview(const math::Vec* start_pos, const math::Vec* end_pos) :
      gui::Widget(
          new gui::layout::DefaultBox(0_px, 0_px)), // TODO: replace stub
      m_startPos(*start_pos),
      m_endPos(*end_pos)
  {
  }

  virtual void draw(sf::RenderTarget&     draw_target,
                    math::TransformStack& transform_stack) override
  {
    const math::Transform& cur_transform = transform_stack.getCoordSystem();

    const math::Point pos1 = cur_transform.transformPoint(m_startPos);
    const math::Point pos2 = cur_transform.transformPoint(m_endPos);

    sf::VertexArray array(sf::Lines, 2);

    array[0] = sf::Vertex(pos1, sf::Color::Blue);
    array[1] = sf::Vertex(pos2, sf::Color::Blue);

    draw_target.draw(array);
  }

private:
  const math::Vec& m_startPos;
  const math::Vec& m_endPos;
};

LineTool::LineTool(const ToolPalette& palette) :
    m_active(false),
    m_preview(new LinePreview(&m_startPos, &m_endPos)),
    m_aligned(false),
    m_startPos(),
    m_endPos(),
    m_palette(palette)
{
}

void LineTool::updateEndPos(const math::Vec& pos)
{
  if (!m_aligned)
  {
    m_endPos = pos;
    return;
  }

  const math::Vec delta = pos - m_startPos;

  if (fabs(delta.x) > fabs(delta.y))
  {
    m_endPos.x = pos.x;
    m_endPos.y = m_startPos.y;
  }
  else
  {
    m_endPos.x = m_startPos.x;
    m_endPos.y = pos.y;
  }
}

void LineTool::onMainButton(ButtonState state, const math::Vec& pos,
                            gui::Canvas& canvas)
{
  if (state == ButtonState::Pressed)
  {
    m_active   = true;
    m_startPos = pos;
    m_endPos   = pos;
  }
  else
  {
    onConfirm(pos, canvas);
  }
}

void LineTool::onModifier1(ButtonState state, const math::Vec& pos,
                           gui::Canvas&)
{
  m_aligned = (state == ButtonState::Pressed);
  if (m_active)
    updateEndPos(pos);
}

void LineTool::onMove(const math::Vec& pos, gui::Canvas&)
{
  if (m_active)
    updateEndPos(pos);
}

void LineTool::onConfirm(const math::Vec& pos, gui::Canvas& canvas)
{
  if (!m_active)
    return;

  updateEndPos(pos);
  m_active = false;

  sf::VertexArray array(sf::Lines, 2);

  array[0] = sf::Vertex(m_startPos, m_palette.getForegroundColor());
  array[1] = sf::Vertex(m_endPos, m_palette.getForegroundColor());

  canvas.getRenderTexture().draw(array);
}

void LineTool::onCancel(const math::Vec&, gui::Canvas&) { m_active = false; }

} // namespace tool
