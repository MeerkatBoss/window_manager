/**
 * @file filter_palette.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __FILTER_FILTER_PALETTE_H
#define __FILTER_FILTER_PALETTE_H

#include <cassert>

#include "filter/brightness_filter.h"
#include "filter/filter.h"
#include "util/dyn_array.h"

namespace filter
{

enum class FilterId
{
  Brightness,
  FilterCount
};

class FilterPalette
{
public:
  FilterPalette() { m_filters.pushBack(new BrightnessFilter()); }

  ~FilterPalette()
  {
    for (size_t i = 0; i < m_filters.getSize(); ++i)
      delete m_filters[i];
  }

  Filter* getLastFilter() { return m_lastFilter; };

  void setLastFilter(size_t filter_id)
  {
    assert(filter_id < getFilterCount());
    m_lastFilter = m_filters[filter_id];
  }

  Filter* getFilter(size_t filter_id)
  {
    assert(filter_id < getFilterCount());
    return m_filters[filter_id];
  }

  size_t getFilterCount() const { return m_filters.getSize(); }

private:
  util::DynArray<Filter*> m_filters;
  Filter*                 m_lastFilter;
};

} // namespace filter

#endif /* filter_palette.h */
