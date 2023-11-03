#include "GUI/Layout/DefaultBox.h"

#include "Math/Vec.h"

namespace gui
{

namespace layout
{

static double getPixels(const Length& len, double parent_len)
{
  if (len.unit == Unit::Percent)
    return parent_len * len.value / 100;
  return (double)len;
}

static Length fromPixels(double pixels, Unit unit, double parent_pixels)
{
  double value = 0;
  switch (unit)
  {
  case Unit::Pixel:
    value = pixels;
    break;
  case Unit::Centimeter:
    value = pixels / PixelsPerInch * CentimetersPerInch;
    break;
  case Unit::Millimeter:
    value =
        pixels / PixelsPerInch * CentimetersPerInch * MillimitersPerCentimeter;
    break;
  case Unit::Inch:
    value = pixels / PixelsPerInch;
    break;
  case Unit::Percent:
    value = pixels / parent_pixels * 100;
    break;
  default:
    value = 0;
    break;
  }

  return Length(value, unit);
}

static double clampToZero(double val)
{
  if (val < 0)
    return 0;
  return val;
}

void DefaultBox::updateParent(const LayoutBox& parent_box)
{
  m_parentSize = parent_box.getSize();
  math::Vec parent_origin(m_parentSize.x * parent_box.getLocalOrigin().x,
                          m_parentSize.y * parent_box.getLocalOrigin().y);

  if (m_align == Align::Free)
  {
    return;
  }

  math::Vec size = getSize();

  const math::Vec origin(m_localOrigin.x * size.x, m_localOrigin.y * size.y);

  math::Vec full_size(getPixels(m_width, m_parentSize.x),
                      getPixels(m_height, m_parentSize.y));

  const double pad_left = getPixels(m_paddingLeft, m_parentSize.x);
  const double pad_top  = getPixels(m_paddingTop, m_parentSize.y);

  const double unit_x = fabs(m_parentSize.x - full_size.x) / 2;
  const double unit_y = fabs(m_parentSize.y - full_size.y) / 2;

  double start_x = m_parentSize.x < full_size.x ? -2 * unit_x : 0;
  double start_y = m_parentSize.y < full_size.y ? -2 * unit_y : 0;

  math::Vec pos(start_x, start_y);

  switch (m_align)
  {
    // clang-format off
  case Align::TopLeft:      pos += math::Vec(       0,        0); break;
  case Align::TopCenter:    pos += math::Vec(  unit_x,        0); break;
  case Align::TopRight:     pos += math::Vec(2*unit_x,        0); break;
  case Align::CenterLeft:   pos += math::Vec(       0,   unit_y); break;
  case Align::Center:       pos += math::Vec(  unit_x,   unit_y); break;
  case Align::CenterRight:  pos += math::Vec(2*unit_x,   unit_y); break;
  case Align::BottomLeft:   pos += math::Vec(       0, 2*unit_y); break;
  case Align::BottomCenter: pos += math::Vec(  unit_x, 2*unit_y); break;
  case Align::BottomRight:  pos += math::Vec(2*unit_x, 2*unit_y); break;
    // clang-format on

  case Align::Free:
  default:
    break;
  }

  pos += origin - parent_origin;

  m_posX = fromPixels(pos.x + pad_left, m_posX.unit, m_parentSize.x);
  m_posY = fromPixels(pos.y + pad_top, m_posY.unit, m_parentSize.y);
}

bool DefaultBox::setSize(const math::Vec& size)
{
  if (!m_resizable)
  {
    return false;
  }

  m_width  = fromPixels(size.x, m_width.unit, m_parentSize.x);
  m_height = fromPixels(size.y, m_height.unit, m_parentSize.y);

  return true;
}

bool DefaultBox::setPosition(const math::Point& position)
{
  if (m_align != Align::Free)
  {
    return false;
  }

  m_posX = fromPixels(position.x, m_posX.unit, m_parentSize.x);
  m_posY = fromPixels(position.y, m_posY.unit, m_parentSize.y);

  return true;
}

bool DefaultBox::setLocalOrigin(const math::Point& position)
{
  m_localOrigin = position;
  return true;
}

math::Vec DefaultBox::getSize() const
{
  const double full_width  = getPixels(m_width, m_parentSize.x);
  const double full_height = getPixels(m_height, m_parentSize.y);

  const double pad_horiz = getPixels(m_paddingLeft, m_parentSize.x) +
                           getPixels(m_paddingRight, m_parentSize.x);
  const double pad_vert = getPixels(m_paddingTop, m_parentSize.y) +
                          getPixels(m_paddingBottom, m_parentSize.y);

  return math::Vec(clampToZero(full_width - pad_horiz),
                   clampToZero(full_height - pad_vert));
}

math::Point DefaultBox::getPosition() const
{
  return math::Point(getPixels(m_posX, m_parentSize.x),
                     getPixels(m_posY, m_parentSize.y));
}

math::Point DefaultBox::getLocalOrigin() const { return m_localOrigin; }

} // namespace layout

} // namespace gui
