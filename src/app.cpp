#include "app.h"

#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cstdio>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "event/event.h"
#include "event/event_emitter.h"
#include "gui/button.h"
#include "gui/frame.h"
#include "gui/window.h"
#include "math/transform.h"
#include "gui/widget.h"

class DebugController : public gui::ButtonController
{
  virtual void onClick(size_t button_id) override
  {
    printf("Clicked %zu!\n", button_id);
    return;
  }
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

  // m_widgetTree = new gui::Button(g_debugController, m_buttonTexture);
  gui::Window* window = new gui::Window(Point(), Vec(1, 1));
  gui::Frame* frame = new gui::Frame(0.05, window);

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

  const math::Vec win_offset(m_window.getSize().x / 2,
                             m_window.getSize().y / 2);
  const double min_offset = win_offset.x < win_offset.y
                                ? win_offset.x
                                : win_offset.y;
  const math::Vec win_scale(min_offset, min_offset);

  stack.enterCoordSystem(math::Transform(win_offset, win_scale));

  size_t counter = 0;
  while (m_window.isOpen())
  {

    if (counter < 10)
    {
      printf("window: %ux%u\n", m_window.getSize().x, m_window.getSize().y);
      ++counter;
    }
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
