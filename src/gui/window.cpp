#include "gui/window.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include "math/transform.h"

namespace gui
{

 void Window::draw(sf::RenderTarget& draw_target,
                    math::TransformStack& transform_stack)
{
  transform_stack.enterCoordSystem(transform());

  const math::Transform& real_transform = transform_stack.getCoordSystem();

  sf::RectangleShape rect(real_transform.getScale());
  rect.setOrigin(real_transform.getScale() / 2);
  rect.setPosition(real_transform.getPosition());
  rect.setFillColor(sf::Color::White);

  draw_target.draw(rect);

  transform_stack.exitCoordSystem();
}

} // namespace gui
