#include "Filter/BlueFilter.h"

namespace filter
{

static inline size_t clamp(size_t val, size_t min, size_t max)
{
  if (val < min)
    return min;
  if (val > max)
    return max;
  return val;
}

void BlueFilter::applyFilter(gui::Canvas& canvas, const FilterMask& mask)
{
  sf::Image image = canvas.getRenderTexture().getTexture().copyToImage();

  for (size_t x = 0; x < mask.getWidth(); ++x)
  {
    for (size_t y = 0; y < mask.getHeight(); ++y)
    {
      if (!mask.getPixel(x, y))
        continue;

      const sf::Color old_color = image.getPixel(x, y);
      const sf::Color new_color = sf::Color(old_color.r, old_color.g,
                                            clamp(old_color.b + 10, 0, 255));
      image.setPixel(x, y, new_color);
    }
  }

  sf::Texture texture;
  texture.loadFromImage(image);
  sf::Sprite sprite(texture);

  canvas.getRenderTexture().draw(sprite);
}


} // namespace filter
