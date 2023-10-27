#include "tool/polyline_tool.h"
#include <SFML/Graphics/Color.hpp>
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
#include "util/dyn_array.h"

namespace tool
{

class PolylinePreview : public gui::Widget
{
public:
  PolylinePreview(const util::DynArray<math::Vec>* points) :
    gui::Widget(new gui::layout::DefaultBox(0_px, 0_px)), // TODO: replace stub
    m_points(*points) {}

  void draw(sf::RenderTarget& draw_target,
            math::TransformStack& transform_stack) override
  {
    const math::Transform& cur_transform = transform_stack.getCoordSystem();
    sf::VertexArray array(sf::LineStrip, m_points.getSize());

    for (size_t i = 0; i < m_points.getSize(); ++i)
    {
      array[i] = sf::Vertex(cur_transform.transformPoint(m_points[i]),
                            sf::Color::Blue);
    }

    draw_target.draw(array);
  }
private:
  const util::DynArray<math::Vec>& m_points;
};

PolylineTool::PolylineTool(const ToolPalette& palette) :
  m_points(), m_active(false),
  m_preview(new PolylinePreview(&m_points)),
  m_palette(palette)
{
  m_points.pushBack(math::Vec());
}

void PolylineTool::onMainButton(ButtonState state,
                                const math::Vec& pos,
                                gui::Canvas& canvas)
{
  if (state != ButtonState::Pressed)
  {
    return;
  }

  m_points[m_points.getSize() - 1] = pos;
  m_points.pushBack(pos);
  const double distance = (m_points[0] - pos).length();

  // TODO: Extract magic constant
  if (!m_active || distance > 5)
  {
    m_active = true;
    return;
  }
  
  m_points[m_points.getSize() - 2] = m_points[0];
  onConfirm(m_points[0], canvas);
}

void PolylineTool::onMove(const math::Vec& pos, gui::Canvas&)
{
  if (m_active)
  {
    m_points[m_points.getSize() - 1] = pos;
  }
}

void PolylineTool::onConfirm(const math::Vec&, gui::Canvas& canvas)
{
  if (!m_active)
  {
    return;
  }

  // m_points[m_points.getSize() - 1] = pos;
  
  sf::VertexArray array(sf::LineStrip, m_points.getSize() - 1);

  for (size_t i = 0; i < m_points.getSize() - 1; ++i)
  {
    array[i] = sf::Vertex(m_points[i], m_palette.getForegroundColor());
  }

  canvas.getRenderTexture().draw(array);

  m_active = false;
  m_points.clear();
  m_points.pushBack(math::Vec());
}

void PolylineTool::onCancel(const math::Vec&, gui::Canvas&)
{
  m_active = false;
  m_points.clear();
  m_points.pushBack(math::Vec());
}

} // namespace tool
