/**
 * @file TextTool.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-07
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __TOOL_TEXT_TOOL_H
#define __TOOL_TEXT_TOOL_H

#include <SFML/Graphics/Font.hpp>

#include "GUI/Widget.h"
#include "Tool/Tool.h"
#include "Util/DynArray.h"

namespace tool
{

class ToolPalette;

class TextTool : public Tool
{
public:
  static constexpr size_t MaxTextLen = 1024;

  TextTool(unsigned font_size_px, const ToolPalette& palette);

  virtual void onMainButton(ButtonState state, const math::Vec& pos,
                            gui::Canvas& canvas) override;

  virtual void onMove(const math::Vec& pos, gui::Canvas& canvas) override;

  virtual void onConfirm(const math::Vec& pos, gui::Canvas& canvas) override;

  virtual void onCancel(const math::Vec& pos, gui::Canvas& canvas) override;

  virtual gui::Widget* getWidget() override
  {
    return m_active ? m_widget : nullptr;
  }

private:
  bool         m_active;
  bool         m_moving;
  gui::Widget* m_widget;
  math::Vec    m_position;

  char            m_text[MaxTextLen + 1];
  size_t          m_textLen;
  const sf::Font& m_font;
  unsigned        m_fontSize;

  const ToolPalette& m_palette;
};

} // namespace tool

#endif /* TextTool.h */
