/**
 * @file tool_widget.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-10-17
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_TOOL_WIDGET_H
#define __GUI_TOOL_WIDGET_H

#include <SFML/Graphics/RenderTarget.hpp>

#include "GUI/Layout/DefaultBox.h"
#include "GUI/WidgetContainer.h"
#include "Math/Transform.h"
#include "Math/TransformStack.h"
#include "Tool/ToolPalette.h"

namespace gui
{

class ToolWidget : public WidgetContainer
{
  using Base = WidgetContainer;

public:
  ToolWidget(Widget* contained, tool::ToolPalette* palette) :
      Base(contained->getLayoutBox()->copy()), m_palette(palette)
  {
    contained->setLayoutBox(new layout::DefaultBox(100_per, 100_per));
    addWidget(contained);
  }

  virtual ~ToolWidget() override { delete m_palette; }

  tool::ToolPalette& getPalette() { return *m_palette; }

  virtual bool onKeyboardPressed(event::KeyboardKey key) override;

private:
  tool::ToolPalette* m_palette;
};

} // namespace gui

#endif /* ToolWidget.h */
