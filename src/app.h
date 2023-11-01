/**
 * @file app.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 * 
 * @brief
 *
 * @version 0.1
 * @date 2023-09-17
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __APP_H
#define __APP_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "gui/widget.h"

class App
{
public:
  static constexpr size_t windowWidth  = 1000;
  static constexpr size_t windowHeight = 1000;

  App();
  ~App();

  void run(void);
private:
  sf::Texture      m_buttonTexture;

  sf::RenderWindow m_window;
  gui::Widget*     m_widgetTree;

  void setupUI();

  void runMainLoop(void);
};

#endif /* app.h */
