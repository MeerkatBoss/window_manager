#include "Tool/RectangleTool.h"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "GUI/Layout/DefaultBox.h"
#include "GUI/Widget.h"
#include "Math/Vec.h"
#include "Tool/ToolPalette.h"

namespace tool
{

class RectPreview : public gui::Widget
{
public:
  RectPreview(const math::Vec* start_pos, const math::Vec* end_pos) :
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

    sf::VertexArray array(sf::LineStrip, 5);

    array[0] = sf::Vertex(pos1, sf::Color::Blue);
    array[1] = sf::Vertex(math::Vec(pos1.x, pos2.y), sf::Color::Blue);
    array[2] = sf::Vertex(pos2, sf::Color::Blue);
    array[3] = sf::Vertex(math::Vec(pos2.x, pos1.y), sf::Color::Blue);
    array[4] = sf::Vertex(pos1, sf::Color::Blue);

    draw_target.draw(array);
  }

private:
  const math::Vec& m_startPos;
  const math::Vec& m_endPos;
};

RectangleTool::RectangleTool(const ToolPalette& palette) :
    m_active(false),
    m_preview(new RectPreview(&m_startPos, &m_endPos)),
    m_square(false),
    m_startPos(),
    m_endPos(),
    m_palette(palette)
{
}

void RectangleTool::updateEndPos(const math::Vec& pos)
{
  if (!m_square)
  {
    m_endPos = pos;
    return;
  }

  const math::Vec delta = pos - m_startPos;

  int sign_x = delta.x < 0 ? -1 : 1;
  int sign_y = delta.y < 0 ? -1 : 1;

  if (fabs(delta.x) > fabs(delta.y))
  {
    m_endPos.x = pos.x;
    m_endPos.y = m_startPos.y + sign_y * fabs(delta.x);
  }
  else
  {
    m_endPos.x = m_startPos.x + sign_x * fabs(delta.y);
    m_endPos.y = pos.y;
  }
}

void RectangleTool::onMainButton(ButtonState state, const math::Vec& pos,
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

void RectangleTool::onModifier1(ButtonState state, const math::Vec& pos,
                                gui::Canvas&)
{
  m_square = (state == ButtonState::Pressed);
  if (m_active)
    updateEndPos(pos);
}

void RectangleTool::onMove(const math::Vec& pos, gui::Canvas&)
{
  if (m_active)
    updateEndPos(pos);
}

void RectangleTool::onConfirm(const math::Vec& pos, gui::Canvas& canvas)
{
  if (!m_active)
    return;

  updateEndPos(pos);
  m_active = false;

  sf::VertexArray array(sf::LineStrip, 5);

  const sf::Color& color = m_palette.getForegroundColor();

  array[0] = sf::Vertex(m_startPos, color);
  array[1] = sf::Vertex(math::Vec(m_startPos.x, m_endPos.y), color);
  array[2] = sf::Vertex(m_endPos, color);
  array[3] = sf::Vertex(math::Vec(m_endPos.x, m_startPos.y), color);
  array[4] = sf::Vertex(m_startPos, color);

  canvas.getRenderTexture().draw(array);
}

void RectangleTool::onCancel(const math::Vec&, gui::Canvas&)
{
  m_active = false;
}

} // namespace tool
