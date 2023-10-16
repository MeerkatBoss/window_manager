#include "tool/ellipse_tool.h"
#include <SFML/Graphics/Vertex.hpp>
#include <cmath>
#include "gui/canvas.h"
#include "gui/widget.h"
#include "math/vec.h"
#include "tool/tool_palette.h"

namespace tool
{

static const size_t ELLIPSE_POINTS = 40;

class EllipsePreview : public gui::Widget
{
public:
  EllipsePreview(const math::Vec* start_pos,
              const math::Vec* end_pos) :
    m_startPos(*start_pos), m_endPos(*end_pos) {}

  virtual void draw(sf::RenderTarget& draw_target,
                    math::TransformStack& transform_stack) override
  {
    const math::Transform& cur_transform = transform_stack.getCoordSystem();

    const math::Point pos1 = cur_transform.transformPoint(m_startPos);
    const math::Point pos2 = cur_transform.transformPoint(m_endPos);

    const math::Point center = (pos1 + pos2) / 2;
    const double x_semi_axis = fabs(pos1.x - center.x);
    const double y_semi_axis = fabs(pos1.y - center.y);
    const double angle_step = 2 * M_PI / ELLIPSE_POINTS;

    sf::VertexArray array(sf::LineStrip, ELLIPSE_POINTS + 1);

    for (size_t i = 0; i <= ELLIPSE_POINTS; ++i)
    {
      const math::Vec pos(center.x + cos(i*angle_step)*x_semi_axis,
                          center.y + sin(i*angle_step)*y_semi_axis);

      array[i] = sf::Vertex(pos, sf::Color::Blue);
    }

    draw_target.draw(array);
  }

private:
  const math::Vec& m_startPos;
  const math::Vec& m_endPos;
};

EllipseTool::EllipseTool(const ToolPalette& palette) : m_active(false),
                       m_preview(new EllipsePreview(&m_startPos, &m_endPos)),
                       m_circle(false), m_startPos(), m_endPos(),
                       m_palette(palette) {}


void EllipseTool::updateEndPos(const math::Vec& pos)
{
  if (!m_circle)
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

void EllipseTool::onMainButton(ButtonState state,
                                 const math::Vec& pos,
                                 gui::Canvas& canvas)
{
  if (state == ButtonState::Pressed)
  {
    m_active = true;
    m_startPos = pos;
    m_endPos = pos;
  }
  else
  {
    onConfirm(pos, canvas);
  }
}

void EllipseTool::onModifier1(ButtonState state,
                                const math::Vec& pos,
                                gui::Canvas&)
{
  m_circle = (state == ButtonState::Pressed);
  if (m_active)
    updateEndPos(pos);
}

void EllipseTool::onMove(const math::Vec& pos, gui::Canvas&)
{
  if (m_active)
    updateEndPos(pos);
}

void EllipseTool::onConfirm(const math::Vec& pos, gui::Canvas& canvas)
{
  if (!m_active)
    return;

  updateEndPos(pos);
  m_active = false;

  const math::Point center = (m_startPos + m_endPos) / 2;
  const double x_semi_axis = fabs(m_startPos.x - center.x);
  const double y_semi_axis = fabs(m_endPos.y - center.y);
  const double angle_step = 2 * M_PI / ELLIPSE_POINTS;

  sf::VertexArray array(sf::LineStrip, ELLIPSE_POINTS + 1);

  for (size_t i = 0; i <= ELLIPSE_POINTS; ++i)
  {
    const math::Vec pos(center.x + cos(i*angle_step)*x_semi_axis,
                        center.y + sin(i*angle_step)*y_semi_axis);

    array[i] = sf::Vertex(pos, m_palette.getForegroundColor());
  }

  canvas.getRenderTexture().draw(array);
}

void EllipseTool::onCancel(const math::Vec&, gui::Canvas&)
{
  m_active = false;
}

} // namespace tool
