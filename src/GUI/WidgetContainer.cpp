#include "GUI/WidgetContainer.h"

#include <cstdio>

#include "Event/Event.h"

namespace gui
{

bool WidgetContainer::onEvent(const event::Event& event)
{
  if (event.isPositionalEvent())
  {
    event.asPositionalEvent()->getTransformStack().enterCoordSystem(
        getLocalTransform());
  }

  bool handled = false;
  for (size_t i = 0; i < m_widgets.getSize(); ++i)
  {
    handled = m_widgets[i]->onEvent(event);
    if (handled)
    {
      break;
    }
  }
  if (event.isPositionalEvent())
  {
    event.asPositionalEvent()->getTransformStack().exitCoordSystem();
  }

  return handled || Widget::onEvent(event);
}

void WidgetContainer::draw(sf::RenderTarget&     draw_target,
                           math::TransformStack& transform_stack)
{
  if (m_widgets.isEmpty())
  {
    return;
  }

  transform_stack.enterCoordSystem(getLocalTransform());
  for (size_t i = m_widgets.getSize(); i > 0; --i)
  {
    m_widgets[i - 1]->draw(draw_target, transform_stack);
  }
  transform_stack.exitCoordSystem();
}

void WidgetContainer::onLayoutUpdate(const layout::LayoutBox& parent_box)
{
  getLayoutBox().updateParent(parent_box);
  size_t widget_count = m_widgets.getSize();
  for (size_t i = 0; i < widget_count; ++i)
  {
    m_widgets[i]->onLayoutUpdate(getLayoutBox());
  }
}

/*
bool WidgetContainer::needEventForward(const event::Event& event) const
{
  size_t type = event.getEventType();
  if (type == event::EventType::MouseMove || type == event::EventType::Update ||
      type == event::EventType::MouseButton)
  {
    return false;
  }

  return true;
}
*/

} // namespace gui
