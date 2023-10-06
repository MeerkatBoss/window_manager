#include "gui/slider.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <cmath>
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

static bool doubleEq(double a, double b)
{
  return fabs(a - b) < 1e-6;
}

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

bool Slider::onMouseMoved(const math::Vec& position,
                          math::TransformStack& transform_stack)
{
  transform_stack.enterCoordSystem(transform());

  const math::Vec local_position = transform_stack.getCoordSystem()
                                                  .restorePoint(position);

  transform_stack.exitCoordSystem();
  const double max_x = .5 - m_handleScale.x/2;
  const double min_x = -max_x;
  const double max_y = .5 - m_handleScale.y/2;
  const double min_y = -max_y;

  const double x_coord = clamp(min_x, max_x, local_position.x);
  const double y_coord = clamp(min_y, max_y, local_position.y);

  const double x_val = doubleEq(min_x, max_x) ? 0 : x_coord / (max_x - min_x);
  const double y_val = doubleEq(min_y, max_y) ? 0 : y_coord / (max_y - min_y);

  m_pendingVal = math::Vec(x_val, y_val);

  if (m_captured)
  {
    m_controller.setValue(getId(), m_pendingVal);
  }

  m_hovered = 
    -0.5 < local_position.x && local_position.x < 0.5 &&
    -0.5 < local_position.y && local_position.y < 0.5;
  
  return m_hovered;
}

void Slider::draw(sf::RenderTarget& draw_target,
                  math::TransformStack& transform_stack)
{
  transform_stack.enterCoordSystem(transform());
  const math::Transform& real_transform = transform_stack.getCoordSystem();

  sf::RectangleShape background(real_transform.getScale());
  background.setOrigin(real_transform.getScale() / 2);
  background.setPosition(real_transform.getPosition());
  background.setFillColor(sf::Color(70, 70, 70));
  draw_target.draw(background);


  const double max_x = .5 - m_handleScale.x/2;
  const double min_x = -max_x;
  const double max_y = .5 - m_handleScale.y/2;
  const double min_y = -max_y;

  const math::Vec slider_val = m_controller.getValue(getId());

  const math::Vec handle_pos =
    real_transform.transformPoint(math::Vec(slider_val.x * (max_x - min_x),
                                            slider_val.y * (max_y - min_y)));
  const math::Vec handle_size = real_transform.transformVector(m_handleScale);

  sf::RectangleShape handle(handle_size);
  handle.setOrigin(handle_size / 2);
  handle.setPosition(handle_pos);
  handle.setFillColor(sf::Color(200, 200, 200));
  draw_target.draw(handle);

  transform_stack.exitCoordSystem();
}


} // namespace gui
