/**
 * @file filter.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 * 
 * @brief
 *
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __FILTER_FILTER_H
#define __FILTER_FILTER_H

#include "gui/canvas.h"
#include "filter/filter_mask.h"

namespace filter
{

class Filter
{
public:
  virtual void applyFilter(gui::Canvas& canvas, const FilterMask& mask) = 0;

  virtual ~Filter() = default;
};

} // namespace filter

#endif /* filter.h */
