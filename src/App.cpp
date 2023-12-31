#include "App.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cstdio>

#include "Assets/AssetManager.h"
#include "Event/Event.h"
#include "Event/EventEmitter.h"
#include "GUI/Button.h"
#include "GUI/Canvas.h"
#include "GUI/Dropdown.h"
#include "GUI/FocusContainer.h"
#include "GUI/Frame.h"
#include "GUI/Layout/DefaultBox.h"
#include "GUI/Layout/LayoutBox.h"
#include "GUI/Layout/Units.h"
#include "GUI/Scrollbar.h"
#include "GUI/Slider.h"
#include "GUI/ToolWidget.h"
#include "GUI/Widget.h"
#include "GUI/WidgetContainer.h"
#include "GUI/WidgetView.h"
#include "Math/Transform.h"
#include "Math/Vec.h"
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

App::App() : m_ended(false)
{
  using math::Transform;
  using math::Vec;

  m_window.create(sf::VideoMode::getDesktopMode(), "Window Manager",
                  sf::Style::Fullscreen);

  setupUI();
}

App::~App()
{
  if (m_window.isOpen())
  {
    m_window.close();
  }
  delete m_widgetTree;
}

void App::onClick(size_t button_id)
{
  if (button_id == m_closeButtonId)
  {
    m_ended = true;
    return;
  }
  if (button_id == m_newCanvasButtonId)
  {
    gui::layout::DefaultBox box(15_cm, 15_cm);
    box.setPosition(
        math::Point(m_window.getSize().x / 2.0, m_window.getSize().y / 2.0));

    gui::Canvas* canvas = new gui::Canvas(*m_tools, m_filters, 800, 800, box);
    gui::Scrollbar* scrollbar = new gui::Scrollbar(1_cm, canvas);
    gui::Frame*     frame     = new gui::Frame(7_mm, scrollbar, "Canvas");

    m_rootContainer->addWidget(frame);
  }
  if (button_id == m_openButtonId)
  {
    sf::Texture tex;
    tex.loadFromFile("assets/mandelbrot.png");
    sf::Sprite sprite(tex);

    const double aspect_ratio =
        double(tex.getSize().x) / double(tex.getSize().y);

    gui::layout::DefaultBox box(
        gui::layout::Length(15 * aspect_ratio, gui::layout::Unit::Centimeter),
        15_cm);
    box.setPosition(
        math::Point(m_window.getSize().x / 2.0, m_window.getSize().y / 2.0));

    gui::Canvas* canvas = new gui::Canvas(*m_tools, m_filters, tex.getSize().x,
                                          tex.getSize().y, box);
    canvas->getRenderTexture().draw(sprite);

    gui::Scrollbar* scrollbar = new gui::Scrollbar(1_cm, canvas);
    gui::Frame*     frame     = new gui::Frame(7_mm, scrollbar, "Canvas");

    m_rootContainer->addWidget(frame);
  }
}

void App::setupUI()
{
  using math::Point;
  using math::Transform;
  using math::Vec;
  using namespace gui;
  using namespace gui::layout;

  m_rootContainer = new FocusContainer(DefaultBox(100_per, 100_per));

  Button* close_button =
      new Button(*this, assets::AssetManager::getButtonTexture(),
                 DefaultBox(1_cm, 1_cm, Align::TopRight), "#");
  m_closeButtonId = close_button->getId();

  Dropdown* file = new Dropdown("File", DefaultBox(5_cm, 1_cm, Align::TopLeft));
  m_newCanvasButtonId = file->addButton(*this, "New");
  m_openButtonId      = file->addButton(*this, "Open");

  WidgetContainer* menu_bar = new WidgetContainer(DefaultBox(100_per, 100_per));
  menu_bar->addWidget(close_button);
  menu_bar->addWidget(file);
  menu_bar->addWidget(m_rootContainer);

  tool::ToolPalette* palette = new tool::ToolPalette();
  gui::ToolWidget*   menu    = new gui::ToolWidget(menu_bar, palette);

  m_tools      = palette;
  m_widgetTree = menu;
}

void App::run() { runMainLoop(); }

void App::runMainLoop()
{
  sf::Event event;

  math::TransformStack stack;
  event::EventEmitter  emitter(m_window, stack);

  const math::Vec win_offset(m_window.getSize().x / 2,
                             m_window.getSize().y / 2);

  gui::layout::DefaultBox root_layout(
      gui::layout::Length(m_window.getSize().x, gui::layout::Unit::Pixel),
      gui::layout::Length(m_window.getSize().y, gui::layout::Unit::Pixel));

  // stack.enterCoordSystem(math::Transform(win_offset));

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

    if (m_ended)
    {
      m_window.close();
    }
  }

  // stack.exitCoordSystem();
}
