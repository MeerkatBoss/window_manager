#include "GUI/Dropdown.h"

#include "Assets/AssetManager.h"
#include "GUI/Button.h"
#include "GUI/Layout/DefaultBox.h"
#include "GUI/Layout/Units.h"

namespace gui
{

size_t Dropdown::addButton(ButtonController& controller, const char* name)
{
  using namespace layout;

  const size_t button_count = m_dropDown.getWidgets().getSize() + 1;

  m_dropDown.setLayoutBox(DefaultBox(
      100_per, Length(100 * (button_count + 1), Unit::Percent), Align::Free));

  Button* added =
      new Button(controller, assets::AssetManager::getButtonTexture(),
                 layout::DefaultBox(0_px, 0_px), name);
  m_dropDown.addWidget(added);

  for (size_t i = 0; i < button_count; ++i)
  {
    DefaultBox button_box(
        100_per, Length((i + 2) * 100.0 / (button_count + 1), Unit::Percent),
        Align::TopCenter);
    button_box.setPadding(
        Length((i + 1) * 100.0 / (button_count + 1), Unit::Percent), 0_px, 0_px,
        0_px);
    m_dropDown.getWidgets()[i]->setLayoutBox(button_box);
  }

  m_dropDown.onLayoutUpdate(getLayoutBox());

  return added->getId();
}

bool Dropdown::onEvent(const event::Event& event)
{
  if (event.isPositionalEvent())
  {
    event.asPositionalEvent()->getTransformStack().enterCoordSystem(
        getLocalTransform());
  }
  bool handled = m_mainButton.onEvent(event);
  if (!handled && m_active)
  {
    handled = m_dropDown.onEvent(event);
  }
  if (event.isPositionalEvent())
  {
    event.asPositionalEvent()->getTransformStack().exitCoordSystem();
  }

  return handled;
}

void Dropdown::draw(sf::RenderTarget&     draw_target,
                    math::TransformStack& transform_stack)
{
  transform_stack.enterCoordSystem(getLocalTransform());
  if (m_active)
    m_dropDown.draw(draw_target, transform_stack);
  m_mainButton.draw(draw_target, transform_stack);
  transform_stack.exitCoordSystem();
}

} // namespace gui
