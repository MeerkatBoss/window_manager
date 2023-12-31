#include "GUI/ToolWidget.h"

#include "Event/Keys.h"

namespace gui
{

bool ToolWidget::onKeyboardPressed(event::KeyboardKey key)
{
  using event::KeyboardKey;

  if (key == KeyboardKey::B)
  {
    m_palette->selectBrushTool();
    return true;
  }

  if (key == KeyboardKey::E)
  {
    m_palette->selectEllipseTool();
    return true;
  }

  if (key == KeyboardKey::F)
  {
    m_palette->selectFillTool();
  }

  if (key == KeyboardKey::L)
  {
    m_palette->selectLineTool();
    return true;
  }

  if (key == KeyboardKey::P)
  {
    m_palette->selectPolylineTool();
  }

  if (key == KeyboardKey::R)
  {
    m_palette->selectRectangleTool();
    return true;
  }

  if (key == KeyboardKey::T)
  {
    m_palette->selectTextTool();
    return true;
  }

  return Base::onKeyboardPressed(key);
}

} // namespace gui
