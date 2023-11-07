#include "GUI/Canvas.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Event/Keys.h"
#include "Filter/Filter.h"
#include "Filter/FilterPalette.h"
#include "GUI/Widget.h"
#include "Math/Transform.h"
#include "Math/TransformStack.h"
#include "Math/Vec.h"
#include "Tool/Tool.h"
#include "Tool/ToolPalette.h"

namespace gui
{

math::Transform Canvas::getTextureTransform() const
{
  const math::Vec size = getSize();
  const math::Vec tex_scale(size.x / m_renderTexture.getSize().x,
                            size.y / m_renderTexture.getSize().y);

  return math::Transform(math::Vec(), tex_scale);
}

bool Canvas::onEvent(const event::Event& event)
{
  Widget* tool_widget = m_palette.getActiveTool()->getWidget();
  if (tool_widget != nullptr)
  {
    bool handled = tool_widget->onEvent(event);
    if (handled)
      return true;
  }

  return Widget::onEvent(event);
}

bool Canvas::onMousePressed(const math::Vec&      position,
                            event::MouseKey       mouse_button,
                            math::TransformStack& transform_stack)
{
  if (!containsPoint(position, transform_stack))
  {
    return false;
  }

  transform_stack.enterCoordSystem(getLocalTransform());
  transform_stack.enterCoordSystem(getTextureTransform());

  const math::Vec local_position =
      transform_stack.getCoordSystem().restorePoint(position);
  transform_stack.exitCoordSystem();
  transform_stack.exitCoordSystem();

  if (mouse_button == event::MouseKey::Left)
  {
    m_palette.getActiveTool()->onMainButton(tool::ButtonState::Pressed,
                                            local_position, *this);
    m_drawing = true;
    return true;
  }
  if (mouse_button == event::MouseKey::Right)
  {
    m_palette.getActiveTool()->onSecondaryButton(tool::ButtonState::Pressed,
                                                 local_position, *this);
    m_drawing = true;
    return true;
  }

  return false;
}

bool Canvas::onMouseReleased(const math::Vec&      position,
                             event::MouseKey       mouse_button,
                             math::TransformStack& transform_stack)
{
  transform_stack.enterCoordSystem(getLocalTransform());
  transform_stack.enterCoordSystem(getTextureTransform());

  const math::Vec local_position =
      transform_stack.getCoordSystem().restorePoint(position);
  transform_stack.exitCoordSystem();
  transform_stack.exitCoordSystem();

  if (mouse_button == event::MouseKey::Left)
  {
    m_palette.getActiveTool()->onMainButton(tool::ButtonState::Released,
                                            local_position, *this);
    m_drawing = false;
    return false;
  }
  if (mouse_button == event::MouseKey::Right)
  {
    m_palette.getActiveTool()->onSecondaryButton(tool::ButtonState::Released,
                                                 local_position, *this);
    m_drawing = false;
    return false;
  }

  return false;
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

  m_palette.getActiveTool()->onMove(local_position, *this);

  return m_drawing;
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

  if (m_control && key == KeyboardKey::R)
  {
    filter::Filter* filter =
      m_filters.getFilter(size_t(filter::FilterId::Red));

    filter->applyFilter(*this, m_mask);

    m_filters.setLastFilter(size_t(filter::FilterId::Red));
    return true;
  }

  if (m_control && key == KeyboardKey::G)
  {
    filter::Filter* filter =
      m_filters.getFilter(size_t(filter::FilterId::Green));

    filter->applyFilter(*this, m_mask);

    m_filters.setLastFilter(size_t(filter::FilterId::Green));
    return true;
  }

  if (m_control && key == KeyboardKey::B)
  {
    filter::Filter* filter =
      m_filters.getFilter(size_t(filter::FilterId::Blue));

    filter->applyFilter(*this, m_mask);

    m_filters.setLastFilter(size_t(filter::FilterId::Blue));
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

  const auto [tl, tr, bl, br] = layout::getRect(getSize());

  const math::Vec tex_size(m_renderTexture.getSize().x,
                           m_renderTexture.getSize().y);
  const auto [tex_tl, tex_tr, tex_bl, tex_br] = layout::getRect(tex_size);

  transform_stack.enterCoordSystem(getLocalTransform());
  const math::Transform& real_transform = transform_stack.getCoordSystem();

  sf::VertexArray array(sf::TriangleStrip, 4);
  array[0] = sf::Vertex(real_transform.transformPoint(tl), tex_tl);
  array[1] = sf::Vertex(real_transform.transformPoint(tr), tex_tr);
  array[2] = sf::Vertex(real_transform.transformPoint(bl), tex_bl);
  array[3] = sf::Vertex(real_transform.transformPoint(br), tex_br);
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
