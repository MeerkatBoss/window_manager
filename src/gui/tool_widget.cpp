#include "gui/tool_widget.h"

#include "event/keys.h"

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

  return Base::onKeyboardPressed(key);
}

} // namespace gui
