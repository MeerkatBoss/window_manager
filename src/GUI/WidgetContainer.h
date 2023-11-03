/**
 * @file widget_decorator.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-10-10
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_WIDGET_CONTAINER_H
#define __GUI_WIDGET_CONTAINER_H

#include "Event/Event.h"
#include "Event/Keys.h"
#include "GUI/Widget.h"
#include "Math/Transform.h"
#include "Util/DynArray.h"

namespace gui
{

class WidgetContainer : public Widget
{
public:
  WidgetContainer(layout::LayoutBox* layout_box) :
      Widget(layout_box), m_widgets(), m_focused(false)
  {
  }

  virtual ~WidgetContainer() override
  {
    for (size_t i = 0; i < m_widgets.getSize(); ++i)
    {
      delete m_widgets[i];
    }
  }

  virtual bool onEvent(const event::Event& event) override;

  virtual bool onMouseMoved(const math::Vec&      position,
                            math::TransformStack& transform_stack) override;

  virtual bool onMouseReleased(event::MouseKey mouse_button) override;

  virtual bool onUpdate(double delta_time) override;

  virtual void draw(sf::RenderTarget&     draw_target,
                    math::TransformStack& transform_stack) override;

  virtual void onLayoutUpdate(const layout::LayoutBox& parent_box) override;

protected:
  bool isFocused(void) const { return m_focused; }

  void focus(void) { m_focused = true; }
  void unfocus(void) { m_focused = false; }

  util::DynArray<Widget*>&       getWidgets() { return m_widgets; }
  const util::DynArray<Widget*>& getWidgets() const
  {
    return const_cast<WidgetContainer*>(this)->getWidgets();
  }

  void addWidget(Widget* widget) { m_widgets.pushBack(widget); }

  bool needEventForward(const event::Event& event) const;

private:
  util::DynArray<Widget*> m_widgets;
  bool                    m_focused;
};

} // namespace gui

#endif /* WidgetDecorator.h */
