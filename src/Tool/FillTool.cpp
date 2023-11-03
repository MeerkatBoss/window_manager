#include "Tool/FillTool.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cstddef>

#include "Tool/Tool.h"
#include "Tool/ToolPalette.h"
#include "Util/DynArray.h"

namespace tool
{

struct Vec2u
{
  size_t x, y;
};

void FillTool::onMainButton(ButtonState state, const math::Vec& pos,
                            gui::Canvas& canvas)
{
  if (state != ButtonState::Pressed)
  {
    return;
  }

  const sf::Color& fg_color = m_palette.getForegroundColor();

  sf::Image image = canvas.getRenderTexture().getTexture().copyToImage();

  const sf::Color start_color = image.getPixel(size_t(pos.x), size_t(pos.y));
  if (start_color == fg_color)
  {
    return;
  }
  const size_t min_x = 0, max_x = canvas.getRenderTexture().getSize().x - 1;
  const size_t min_y = 0, max_y = canvas.getRenderTexture().getSize().y - 1;

  util::DynArray<Vec2u> to_visit;
  to_visit.pushBack({.x = size_t(pos.x), .y = size_t(pos.y)});
  image.setPixel(size_t(pos.x), size_t(pos.y), fg_color);

  while (!to_visit.isEmpty())
  {
    Vec2u cur = to_visit.back();
    to_visit.popBack();

    if (cur.x < max_x && image.getPixel(cur.x + 1, cur.y) == start_color)
    {
      to_visit.pushBack({cur.x + 1, cur.y});
      image.setPixel(cur.x + 1, cur.y, fg_color);
    }
    if (cur.x > min_x && image.getPixel(cur.x - 1, cur.y) == start_color)
    {
      to_visit.pushBack({cur.x - 1, cur.y});
      image.setPixel(cur.x - 1, cur.y, fg_color);
    }
    if (cur.y < max_y && image.getPixel(cur.x, cur.y + 1) == start_color)
    {
      to_visit.pushBack({cur.x, cur.y + 1});
      image.setPixel(cur.x, cur.y + 1, fg_color);
    }
    if (cur.y > min_y && image.getPixel(cur.x, cur.y - 1) == start_color)
    {
      to_visit.pushBack({cur.x, cur.y - 1});
      image.setPixel(cur.x, cur.y - 1, fg_color);
    }
  }

  sf::Texture texture;
  texture.loadFromImage(image);
  sf::Sprite sprite(texture);

  canvas.getRenderTexture().draw(sprite);
}

} // namespace tool
