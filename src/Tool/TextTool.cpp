#include "Tool/TextTool.h"

#include <SFML/Graphics/Font.hpp>
#include <cctype>
#include <cstdio>
#include <cstring>

#include "Assets/AssetManager.h"
#include "Event/Keys.h"
#include "GUI/Layout/DefaultBox.h"
#include "GUI/Widget.h"
#include "Tool/ToolPalette.h"
#include "Util/DynArray.h"

namespace tool
{

static int asCharacter(event::KeyboardKey key, bool shift);

class TextEditWidget : public gui::Widget
{
public:
  TextEditWidget(const math::Vec* position,
                 char (*text)[TextTool::MaxTextLen + 1], size_t* text_len,
                 unsigned font_size_px) :
      Widget(gui::layout::DefaultBox(0_px, 0_px)), // TODO: Replace stub
      m_shift(false),
      m_position(*position),
      m_text(*text),
      m_textLen(*text_len),
      m_font(assets::AssetManager::getDefaultFont()),
      m_fontSizePx(font_size_px)
  {
  }

  virtual bool onKeyboardPressed(event::KeyboardKey key) override
  {
    if (key == event::KeyboardKey::LShift || key == event::KeyboardKey::RShift)
    {
      m_shift = true;
      return true;
    }
    if (key == event::KeyboardKey::Backspace)
    {
      if (m_textLen > 0)
      {
        --m_textLen;
        m_text[m_textLen] = '\0';
      }

      return true;
    }
   

    const int received = asCharacter(key, m_shift);
    if (received != EOF && m_textLen < TextTool::MaxTextLen)
    {
      m_text[m_textLen] = char(received);
      ++m_textLen;
      return true;
    }
    return false;
  }

  virtual bool onKeyboardReleased(event::KeyboardKey key) override
  {
    if (key == event::KeyboardKey::LShift || key == event::KeyboardKey::RShift)
    {
      m_shift = false;
    }

    return false;
  }

  virtual void draw(sf::RenderTarget&     draw_target,
                    math::TransformStack& transform_stack) override
  {
    sf::Text drawn_text(m_text, m_font);
    drawn_text.setCharacterSize(m_fontSizePx);
    drawn_text.setPosition(
        transform_stack.getCoordSystem().transformPoint(m_position));
    drawn_text.setScale(transform_stack.getCoordSystem().getScale());
    drawn_text.setFillColor(sf::Color::Blue);

    draw_target.draw(drawn_text);
  }

private:
  bool             m_shift;
  const math::Vec& m_position;
  char (&m_text)[TextTool::MaxTextLen + 1];
  size_t&         m_textLen;
  const sf::Font& m_font;
  unsigned        m_fontSizePx;
};

TextTool::TextTool(unsigned font_size_px, const ToolPalette& palette) :
    m_active(false),
    m_moving(false),
    m_widget(
        new TextEditWidget(&m_position, &m_text, &m_textLen, font_size_px)),
    m_position(0, 0),
    m_text(),
    m_textLen(0),
    m_font(assets::AssetManager::getDefaultFont()),
    m_fontSize(font_size_px),
    m_palette(palette)
{
  memset(m_text, 0, sizeof(m_text));
}

void TextTool::onMainButton(ButtonState state, const math::Vec& pos,
                            gui::Canvas&)
{
  if (state == ButtonState::Pressed)
  {
    m_position = pos;
    m_active   = true;
    m_moving   = true;
  }
  else
  {
    m_moving = false;
  }
}

void TextTool::onMove(const math::Vec& pos, gui::Canvas&)
{
  if (m_moving)
  {
    m_position = pos;
  }
}

void TextTool::onConfirm(const math::Vec&, gui::Canvas& canvas)
{
  sf::Text drawn_text(m_text, m_font);
  drawn_text.setCharacterSize(m_fontSize);
  drawn_text.setPosition(m_position);
  drawn_text.setFillColor(m_palette.getForegroundColor());
  canvas.getRenderTexture().draw(drawn_text);

  m_active = false;
  m_moving = false;
  memset(m_text, 0, sizeof(m_text));
  m_textLen = 0;
}

void TextTool::onCancel(const math::Vec&, gui::Canvas&)
{
  m_active = false;
  m_moving = false;
  memset(m_text, 0, sizeof(m_text));
  m_textLen = 0;
}

static int asCharacter(event::KeyboardKey key, bool shift)
{
  using event::KeyboardKey;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"

#define CONVERT_EVENT(ch)                                                      \
  case KeyboardKey::ch:                                                        \
    return (shift ? *#ch : tolower(*#ch))

  switch (key)
  {
    CONVERT_EVENT(A);
    CONVERT_EVENT(B);
    CONVERT_EVENT(C);
    CONVERT_EVENT(D);
    CONVERT_EVENT(E);
    CONVERT_EVENT(F);
    CONVERT_EVENT(G);
    CONVERT_EVENT(H);
    CONVERT_EVENT(I);
    CONVERT_EVENT(J);
    CONVERT_EVENT(K);
    CONVERT_EVENT(L);
    CONVERT_EVENT(M);
    CONVERT_EVENT(N);
    CONVERT_EVENT(O);
    CONVERT_EVENT(P);
    CONVERT_EVENT(Q);
    CONVERT_EVENT(R);
    CONVERT_EVENT(S);
    CONVERT_EVENT(T);
    CONVERT_EVENT(U);
    CONVERT_EVENT(V);
    CONVERT_EVENT(W);
    CONVERT_EVENT(X);
    CONVERT_EVENT(Y);
    CONVERT_EVENT(Z);

  case KeyboardKey::Space:
    return ' ';
  case KeyboardKey::Num1:
    return (shift ? '!' : '1');
  case KeyboardKey::Num2:
    return (shift ? '@' : '2');
  case KeyboardKey::Num3:
    return (shift ? '#' : '3');
  case KeyboardKey::Num4:
    return (shift ? '$' : '4');
  case KeyboardKey::Num5:
    return (shift ? '%' : '5');
  case KeyboardKey::Num6:
    return (shift ? '^' : '6');
  case KeyboardKey::Num7:
    return (shift ? '&' : '7');
  case KeyboardKey::Num8:
    return (shift ? '*' : '8');
  case KeyboardKey::Num9:
    return (shift ? '(' : '9');
  case KeyboardKey::Num0:
    return (shift ? ')' : '0');
  case KeyboardKey::Comma:
    return (shift ? '<' : ',');
  case KeyboardKey::Period:
    return (shift ? '>' : '.');
  // TODO: Convert the rest of events
  default:
    return EOF;
  }

#undef CONVERT_EVENT

#pragma clang diagnostic pop
}

} // namespace tool
