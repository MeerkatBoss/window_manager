#include "App.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cstdio>

#include "Event/Event.h"
#include "Event/EventEmitter.h"
#include "GUI/Button.h"
#include "GUI/Canvas.h"
#include "GUI/Frame.h"
#include "GUI/Layout/DefaultBox.h"
#include "GUI/Scrollbar.h"
#include "GUI/Slider.h"
#include "GUI/ToolWidget.h"
#include "GUI/Widget.h"
#include "GUI/WidgetView.h"
#include "Math/Transform.h"
#include "Tool/BrushTool.h"
#include "Tool/EllipseTool.h"
#include "Tool/FillTool.h"
#include "Tool/LineTool.h"
#include "Tool/PolylineTool.h"
#include "Tool/RectangleTool.h"
#include "Tool/ToolPalette.h"

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
    printf("#%zu = (%lg, %lg)\n", slider_id, val.x, val.y);
    m_sliderVal = val;
  }
  virtual math::Vec getValue(size_t) override { return m_sliderVal; }

private:
  math::Vec m_sliderVal;
};

static DebugController g_debugController;

App::App()
{
  using math::Transform;
  using math::Vec;

  m_window.create(sf::VideoMode::getDesktopMode(), "Window Manager",
                  sf::Style::Fullscreen);

  m_buttonTexture.loadFromFile("assets/button_square.png");

  setupUI();
}

void App::setupUI()
{
  using math::Point;
  using math::Transform;
  using math::Vec;

  tool::ToolPalette* palette = new tool::ToolPalette();

  gui::Canvas* canvas = new gui::Canvas(
      *palette, m_filters, 800, 800, new gui::layout::DefaultBox(15_cm, 15_cm));
  gui::Scrollbar* scrollbar = new gui::Scrollbar(1_cm, canvas, m_buttonTexture);
  gui::ToolWidget* menu     = new gui::ToolWidget(scrollbar, palette);
  gui::Frame*      frame    = new gui::Frame(7_mm, menu, m_buttonTexture);

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

void App::run() { runMainLoop(); }

void App::runMainLoop()
{
  sf::Event event;

  math::TransformStack stack;
  event::EventEmitter  emitter(stack);

  const math::Vec win_offset(m_window.getSize().x / 2,
                             m_window.getSize().y / 2);

  gui::layout::DefaultBox root_layout(
      gui::layout::Length(m_window.getSize().x, gui::layout::Unit::Pixel),
      gui::layout::Length(m_window.getSize().y, gui::layout::Unit::Pixel));

  stack.enterCoordSystem(math::Transform(win_offset));

  m_widgetTree->onLayoutUpdate(root_layout);
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
