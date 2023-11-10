#include "GUI/FocusContainer.h"

namespace gui
{

bool FocusContainer::onEvent(const event::Event& event)
{
  if (event.isPositionalEvent())
  {
    event.asPositionalEvent()->getTransformStack().enterCoordSystem(
        getLocalTransform());
  }
  bool handled = false;

  if (m_focused != nullptr)
  {
    handled = m_focused->onEvent(event);
  }
  for (size_t i = 0; !handled && i < getWidgets().getSize(); ++i)
  {
    handled = getWidgets()[i]->onEvent(event);
    if (handled)
    {
      m_focused = getWidgets()[i];
    }
  }

  if (event.isPositionalEvent())
  {
    event.asPositionalEvent()->getTransformStack().exitCoordSystem();
  }

  return handled || Widget::onEvent(event);
}

void FocusContainer::draw(sf::RenderTarget&     draw_target,
                          math::TransformStack& transform_stack)
{
  if (getWidgets().isEmpty())
  {
    return;
  }

  transform_stack.enterCoordSystem(getLocalTransform());

  for (size_t i = getWidgets().getSize(); i > 0; --i)
  {
    if (getWidgets()[i - 1] != m_focused)
    {
      getWidgets()[i - 1]->draw(draw_target, transform_stack);
    }
  }
  if (m_focused != nullptr)
  {
    m_focused->draw(draw_target, transform_stack);
  }

  transform_stack.exitCoordSystem();
}

} // namespace gui
