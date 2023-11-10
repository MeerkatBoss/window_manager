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

#include "Filter/FilterPalette.h"
#include "GUI/Button.h"
#include "GUI/Canvas.h"
#include "GUI/FocusContainer.h"
#include "GUI/Widget.h"

class App : private gui::ButtonController
{
public:
  App();
  ~App();

  void run(void);

private:
  sf::RenderWindow      m_window;
  gui::Widget*          m_widgetTree;
  gui::FocusContainer*  m_rootContainer;
  filter::FilterPalette m_filters;
  tool::ToolPalette*    m_tools;
  bool                  m_ended;

  size_t m_closeButtonId, m_newCanvasButtonId, m_openButtonId;

  virtual void onClick(size_t button_id) override;

  void setupUI();

  void runMainLoop(void);
};

#endif /* App.h */
