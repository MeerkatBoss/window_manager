#include "app.h"

#include <cstdio>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "math/transform.h"
#include "gui/widget.h"

App::App()
{
  using math::Vec;
  using math::Transform;

  m_window.create(sf::VideoMode(windowWidth, windowHeight),
                  "Window Manager",
                  sf::Style::Close);

  setupUI();
}

void App::setupUI()
{
  using math::Vec;
  using math::Point;
  using math::Transform;
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
  sf::Clock clock;

  math::Transform root = math::Transform(math::Vec(0, 0),
                                       math::Vec(windowWidth, windowHeight));
  while (m_window.isOpen())
  {
    while (m_window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        m_window.close();
      }
    }
    if (!m_window.isOpen())
    {
      break;
    }
    double delta_time = clock.restart().asSeconds();

    m_window.clear(sf::Color(128, 128, 128));

    m_widgetTree->draw(m_window, root);
    m_window.display();
  }

}
