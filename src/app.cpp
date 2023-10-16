#include "app.h"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cstdio>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "event/event.h"
#include "event/event_emitter.h"
#include "gui/button.h"
#include "gui/canvas.h"
#include "gui/frame.h"
#include "gui/slider.h"
#include "gui/tool_widget.h"
#include "gui/window.h"
#include "math/transform.h"
#include "gui/widget.h"
#include "gui/scrollbar.h"
#include "tool/brush_tool.h"
#include "tool/ellipse_tool.h"
#include "tool/fill_tool.h"
#include "tool/line_tool.h"
#include "tool/polyline_tool.h"
#include "tool/rectangle_tool.h"
#include "tool/tool_palette.h"

class DebugController : public gui::ButtonController,
                        public gui::SliderController
{
  virtual void onClick(size_t button_id) override
  {
    printf("Clicked %zu!\n", button_id);
    return;
  }

  virtual void setValue(size_t slider_id, const math::Vec& val) override
  {
    printf("%zu = (%lg, %lg)\n", slider_id, val.x, val.y);
    m_sliderVal = val;
  }
  virtual math::Vec getValue(size_t) override { return m_sliderVal; }
private:
  math::Vec m_sliderVal;
};

static DebugController g_debugController;

App::App()
{
  using math::Vec;
  using math::Transform;

  m_window.create(sf::VideoMode::getDesktopMode(),
                  "Window Manager",
                  sf::Style::Fullscreen);

  m_buttonTexture.loadFromFile("assets/button_square.png");

  setupUI();
}

void App::setupUI()
{
  using math::Vec;
  using math::Point;
  using math::Transform;

  tool::ToolPalette* palette = new tool::ToolPalette();

  gui::Canvas* canvas = new gui::Canvas(*palette, 800, 800,
                                        Point(), Vec(2, 2));
  gui::Scrollbar* scrollbar = new gui::Scrollbar(Transform(), 0.05,
                                                 canvas, m_buttonTexture);
  gui::ToolWidget* menu = new gui::ToolWidget(scrollbar, palette);
  gui::Frame* frame = new gui::Frame(0.07, menu, m_buttonTexture);

  m_widgetTree = frame;
}

App::~App()
{
  if (m_window.isOpen())
  {
    m_window.close();
  }
  delete m_widgetTree;
}

void App::run()
{
  runMainLoop();
}

void App::runMainLoop()
{
  sf::Event event;

  math::TransformStack stack;
  event::EventEmitter emitter(stack);
  sf::RenderTexture texture;
  texture.create(m_window.getSize().x, m_window.getSize().y);

  const math::Vec win_offset(m_window.getSize().x / 2,
                             m_window.getSize().y / 2);
  const double min_offset = win_offset.x < win_offset.y
                                ? win_offset.x
                                : win_offset.y;
  const math::Vec win_scale(min_offset, min_offset);

  stack.enterCoordSystem(math::Transform(win_offset, win_scale));

  while (m_window.isOpen())
  {
    while (m_window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        m_window.close();
      }
      else
      {
        event::Event* emitted = emitter.emitEvent(event);

        if (emitted)
        {
          m_widgetTree->onEvent(*emitted);
          delete emitted;
        }
      }
    }

    if (!m_window.isOpen())
    {
      break;
    }

    m_widgetTree->onEvent(emitter.emitUpdateEvent());

    m_window.clear(sf::Color(128, 128, 128));

    m_widgetTree->draw(m_window, stack);

    m_window.display();

  }

  stack.exitCoordSystem();
}
