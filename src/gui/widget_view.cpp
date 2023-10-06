#include "gui/widget_view.h"

namespace gui
{

void WidgetView::draw(sf::RenderTarget& draw_target,
                      math::TransformStack& transform_stack)
{
  if (m_viewTexture.getSize().x < draw_target.getSize().x ||
      m_viewTexture.getSize().y < draw_target.getSize().y)
  {
    m_viewTexture.create(draw_target.getSize().x, 
                           draw_target.getSize().y);
  }

  transform_stack.enterCoordSystem(transform());

  m_viewTexture.clear(sf::Color(100, 100, 100));
  getDecorated()->draw(m_viewTexture, transform_stack);
  m_viewTexture.display();

  const math::Transform& real_transform = transform_stack.getCoordSystem();
  const math::Vec& position = real_transform.getPosition();
  const math::Vec& scale = real_transform.getScale();

  const double left   = position.x - scale.x / 2;
  const double right  = position.x + scale.x / 2;
  const double top    = position.y - scale.y / 2;
  const double bottom = position.y + scale.y / 2;

  sf::VertexArray array(sf::TriangleStrip, 4);
  array[0] = sf::Vertex(math::Vec(left,  top),    math::Vec(left,  top));
  array[1] = sf::Vertex(math::Vec(right, top),    math::Vec(right, top));
  array[2] = sf::Vertex(math::Vec(left,  bottom), math::Vec(left,  bottom));
  array[3] = sf::Vertex(math::Vec(right, bottom), math::Vec(right, bottom));
  sf::RenderStates states;
  states.texture = &m_viewTexture.getTexture();

  draw_target.draw(array, states);

  transform_stack.exitCoordSystem();
}

} // namespace gui
