/**
 * @file brightness_filter.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __FILTER_BRIGHTNESS_FILTER_H
#define __FILTER_BRIGHTNESS_FILTER_H

#include "Filter/Filter.h"

namespace filter
{

class BrightnessFilter : public Filter
{
public:
  virtual void applyFilter(gui::Canvas&      canvas,
                           const FilterMask& mask) override;
};

} // namespace filter

#endif /* BrightnessFilter.h */
