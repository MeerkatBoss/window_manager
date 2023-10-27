#include "gui/slider.h"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <cmath>
#include <cstdio>

#include "event/keys.h"
#include "math/transform.h"

namespace gui
{

static double clamp(double left, double right, double x)
{
  if (x < left)
    return left;
  if (x > right)
    return right;
  return x;
}

static bool doubleEq(double a, double b) { return fabs(a - b) < 1e-6; }

bool Slider::onMousePressed(event::MouseKey mouse_button)
{
  if (mouse_button != event::MouseKey::Left || !m_hovered)
    return false;

  m_captured = true;
  m_controller.setValue(getId(), m_pendingVal);

  return true;
}

bool Slider::onMouseReleased(event::MouseKey mouse_button)
{
  if (mouse_button != event::MouseKey::Left || !m_captured)
    return false;

  m_captured = false;

  return true;
}

bool Slider::onMouseMoved(const math::Vec&      position,
                          math::TransformStack& transform_stack)
{
  m_hovered = containsPoint(position, transform_stack);

  transform_stack.enterCoordSystem(getLocalTransform());

  // TODO: Extract to getScaledOrigin
  const math::Vec size = getSize();
  const math::Vec origin(getLayoutBox()->getLocalOrigin().x * size.x,
                         getLayoutBox()->getLocalOrigin().y * size.y);
  const math::Vec handle_size(size.x * m_handleScale.x,
                              size.y * m_handleScale.y);

  const math::Vec local_position =
      transform_stack.getCoordSystem().restorePoint(position) + origin;

  transform_stack.exitCoordSystem();

  const double max_x = size.x - handle_size.x;
  const double min_x = 0;
  const double max_y = size.y - handle_size.y;
  const double min_y = 0;

  const double x_coord = clamp(min_x, max_x, local_position.x - handle_size.x/2);
  const double y_coord = clamp(min_y, max_y, local_position.y - handle_size.y/2);

  const double x_val = doubleEq(min_x, max_x) ? 0 : x_coord / (max_x - min_x);
  const double y_val = doubleEq(min_y, max_y) ? 0 : y_coord / (max_y - min_y);

  m_pendingVal = math::Vec(x_val, y_val);

  if (m_captured)
  {
    m_controller.setValue(getId(), m_pendingVal);
  }

  return m_hovered;
}

void Slider::draw(sf::RenderTarget&     draw_target,
                  math::TransformStack& transform_stack)
{
  transform_stack.enterCoordSystem(getLocalTransform());

  drawBackground(draw_target, transform_stack);
  drawHandle(draw_target, transform_stack);

  transform_stack.exitCoordSystem();
}

void Slider::drawBackground(sf::RenderTarget&     draw_target,
                            math::TransformStack& transform_stack)
{
  const math::Transform& transform = transform_stack.getCoordSystem();
  const math::Vec        size      = getSize();
  const math::Vec        origin(getLayoutBox()->getLocalOrigin().x * size.x,
                         getLayoutBox()->getLocalOrigin().y * size.y);

  const math::Point tl(0, 0);
  const math::Point tr(size.x, 0);
  const math::Point bl(0, size.y);
  const math::Point br(size.x, size.y);

  sf::VertexArray array(sf::TriangleStrip, 4);

  const sf::Color color(70, 70, 70);

  array[0] = sf::Vertex(transform.transformPoint(tl - origin), color);
  array[1] = sf::Vertex(transform.transformPoint(tr - origin), color);
  array[2] = sf::Vertex(transform.transformPoint(bl - origin), color);
  array[3] = sf::Vertex(transform.transformPoint(br - origin), color);

  draw_target.draw(array);
}

void Slider::drawHandle(sf::RenderTarget&     draw_target,
                        math::TransformStack& transform_stack)
{
  const math::Transform& transform = transform_stack.getCoordSystem();
  const math::Vec        size      = getSize();

  const math::Vec origin(getLayoutBox()->getLocalOrigin().x * size.x,
                         getLayoutBox()->getLocalOrigin().y * size.y);

  const math::Vec handle_size(size.x * m_handleScale.x,
                              size.y * m_handleScale.y);

  const double max_x = size.x - handle_size.x;
  const double min_x = 0;
  const double max_y = size.y - handle_size.y;
  const double min_y = 0;

  const math::Vec value = m_controller.getValue(getId());
  const math::Vec handle_pos(value.x * (max_x - min_x),
                             value.y * (max_y - min_y));

  const math::Point tl(0, 0);
  const math::Point tr(handle_size.x, 0);
  const math::Point bl(0, handle_size.y);
  const math::Point br(handle_size.x, handle_size.y);

  sf::VertexArray array(sf::TriangleStrip, 4);

  const sf::Color color(200, 200, 200);

  array[0] =
      sf::Vertex(transform.transformPoint(tl + handle_pos - origin), color);
  array[1] =
      sf::Vertex(transform.transformPoint(tr + handle_pos - origin), color);
  array[2] =
      sf::Vertex(transform.transformPoint(bl + handle_pos - origin), color);
  array[3] =
      sf::Vertex(transform.transformPoint(br + handle_pos - origin), color);

  draw_target.draw(array);
}

} // namespace gui
