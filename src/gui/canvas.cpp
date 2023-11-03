#include "gui/canvas.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "event/keys.h"
#include "filter/filter.h"
#include "filter/filter_palette.h"
#include "gui/widget.h"
#include "math/transform.h"
#include "math/transform_stack.h"
#include "math/vec.h"
#include "tool/tool.h"
#include "tool/tool_palette.h"

namespace gui
{

math::Transform Canvas::getTextureTransform() const
{
  const math::Vec size = getSize();
  const math::Vec origin(getLayoutBox()->getLocalOrigin().x * size.x,
                         getLayoutBox()->getLocalOrigin().y * size.y);

  const math::Vec tex_scale(size.x / m_renderTexture.getSize().x,
                            size.y / m_renderTexture.getSize().y);

  return math::Transform(-origin, tex_scale);
}

bool Canvas::onMousePressed(event::MouseKey mouse_button)
{
  if (mouse_button == event::MouseKey::Right)
  {
    m_palette.getActiveTool()->onSecondaryButton(tool::ButtonState::Pressed,
                                                 m_lastPos, *this);
  }

  if (mouse_button != event::MouseKey::Left || !m_hovered)
    return false;

  m_palette.getActiveTool()->onMainButton(tool::ButtonState::Pressed, m_lastPos,
                                          *this);

  return true;
}

bool Canvas::onMouseReleased(event::MouseKey mouse_button)
{
  if (mouse_button == event::MouseKey::Right)
  {
    m_palette.getActiveTool()->onSecondaryButton(tool::ButtonState::Released,
                                                 m_lastPos, *this);
  }

  if (mouse_button != event::MouseKey::Left)
    return false;

  m_palette.getActiveTool()->onMainButton(tool::ButtonState::Released,
                                          m_lastPos, *this);

  return true;
}

bool Canvas::onMouseMoved(const math::Vec&      position,
                          math::TransformStack& transform_stack)
{
  transform_stack.enterCoordSystem(getLocalTransform());
  transform_stack.enterCoordSystem(getTextureTransform());

  const math::Vec local_position =
      transform_stack.getCoordSystem().restorePoint(position);
  transform_stack.exitCoordSystem();
  transform_stack.exitCoordSystem();

  double max_x = m_renderTexture.getSize().x;
  double max_y = m_renderTexture.getSize().y;

  m_hovered = (0 < local_position.x && local_position.x < max_x &&
               0 < local_position.y && local_position.y < max_y);

  if (m_hovered)
    m_lastPos = local_position;

  m_palette.getActiveTool()->onMove(m_lastPos, *this);

  return true;
}

bool Canvas::onKeyboardPressed(event::KeyboardKey key)
{
  using event::KeyboardKey;

  if (key == KeyboardKey::LShift || key == KeyboardKey::RShift)
  {
    m_palette.getActiveTool()->onModifier1(tool::ButtonState::Pressed,
                                           m_lastPos, *this);
    return true;
  }

  if (key == KeyboardKey::LControl || key == KeyboardKey::RControl)
  {
    m_palette.getActiveTool()->onModifier2(tool::ButtonState::Pressed,
                                           m_lastPos, *this);
    m_control = true;
    return true;
  }

  if (key == KeyboardKey::LAlt || key == KeyboardKey::RAlt)
  {
    m_palette.getActiveTool()->onModifier3(tool::ButtonState::Pressed,
                                           m_lastPos, *this);
    return true;
  }

  if (key == KeyboardKey::Escape)
  {
    m_palette.getActiveTool()->onCancel(m_lastPos, *this);
    return true;
  }

  if (key == KeyboardKey::Enter)
  {
    m_palette.getActiveTool()->onConfirm(m_lastPos, *this);
    return true;
  }

  if (m_control && key == KeyboardKey::F)
  {
    filter::Filter* last_filter = m_filters.getLastFilter();
    if (last_filter)
      last_filter->applyFilter(*this, m_mask);
    return true;
  }

  if (m_control && key == KeyboardKey::W)
  {
    filter::Filter* filter =
        m_filters.getFilter(size_t(filter::FilterId::Brightness));

    filter->applyFilter(*this, m_mask);

    m_filters.setLastFilter(size_t(filter::FilterId::Brightness));
    return true;
  }

  return false;
}

bool Canvas::onKeyboardReleased(event::KeyboardKey key)
{
  using event::KeyboardKey;

  if (key == KeyboardKey::LShift || key == KeyboardKey::RShift)
  {
    m_palette.getActiveTool()->onModifier1(tool::ButtonState::Released,
                                           m_lastPos, *this);
    return true;
  }

  if (key == KeyboardKey::LControl || key == KeyboardKey::RControl)
  {
    m_palette.getActiveTool()->onModifier2(tool::ButtonState::Released,
                                           m_lastPos, *this);
    m_control = false;
    return true;
  }

  if (key == KeyboardKey::LAlt || key == KeyboardKey::RAlt)
  {
    m_palette.getActiveTool()->onModifier3(tool::ButtonState::Released,
                                           m_lastPos, *this);
    return true;
  }

  return false;
}

void Canvas::draw(sf::RenderTarget&     draw_target,
                  math::TransformStack& transform_stack)
{
  m_renderTexture.display();

  const auto [tl, tr, bl, br] = layout::getRect(getLayoutBox()->getSize());

  const math::Vec tex_size(m_renderTexture.getSize().x,
                           m_renderTexture.getSize().y);
  const auto [tex_tl, tex_tr, tex_bl, tex_br] = layout::getRect(tex_size);

  const math::Vec origin = layout::getAbsoluteOrigin(getLayoutBox());

  transform_stack.enterCoordSystem(getLocalTransform());
  const math::Transform& real_transform = transform_stack.getCoordSystem();

  sf::VertexArray array(sf::TriangleStrip, 4);
  array[0] = sf::Vertex(real_transform.transformPoint(tl - origin), tex_tl);
  array[1] = sf::Vertex(real_transform.transformPoint(tr - origin), tex_tr);
  array[2] = sf::Vertex(real_transform.transformPoint(bl - origin), tex_bl);
  array[3] = sf::Vertex(real_transform.transformPoint(br - origin), tex_br);
  draw_target.draw(array, &m_renderTexture.getTexture());

  Widget* tool_widget = m_palette.getActiveTool()->getWidget();
  if (tool_widget)
  {
    transform_stack.enterCoordSystem(getTextureTransform());
    tool_widget->draw(draw_target, transform_stack);
    transform_stack.exitCoordSystem();
  }

  transform_stack.exitCoordSystem();
}

} // namespace gui
