/**
 * @file filter_mask.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 * 
 * @brief
 *
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __FILTER_FILTER_MASK_H
#define __FILTER_FILTER_MASK_H

#include <cstddef>

namespace filter
{

class FilterMask
{
public:
  FilterMask(size_t width, size_t height) :
    m_mask(new bool[width * height]),
    m_width(width),
    m_height(height)
  {
    fill(false);
  }

  ~FilterMask()
  {
    delete[] m_mask;
  }

  size_t getWidth() const { return m_width; }
  size_t getHeight() const { return m_height; }

  bool getPixel(size_t x, size_t y) const
  {
    return m_mask[y * m_width + x];
  }
  void setPixel(size_t x, size_t y, bool value)
  {
    m_mask[y * m_width + x] = value;
  }

  void fill(bool value)
  {
    for (size_t x = 0; x < m_width; ++x)
    {
      for (size_t y = 0; y < m_height; ++y)
      {
        setPixel(x, y, value);
      }
    }
  }

  void invert()
  {
    for (size_t x = 0; x < m_width; ++x)
    {
      for (size_t y = 0; y < m_height; ++y)
      {
        const bool old_val = getPixel(x, y);
        setPixel(x, y, !old_val);
      }
    }
  }

private:
  bool*  m_mask;
  size_t m_width;
  size_t m_height;
};

} // namespace filter

#endif /* filter_mask.h */
