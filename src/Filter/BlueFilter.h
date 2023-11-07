/**
 * @file BlueFilter.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-08
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __FILTER_BLUE_FILTER_H
#define __FILTER_BLUE_FILTER_H

#include "Filter/Filter.h"
namespace filter
{

class BlueFilter : public Filter
{
public:
  virtual void applyFilter(gui::Canvas&      canvas,
                           const FilterMask& mask) override;
};

} // namespace filter


#endif /* BlueFilter.h */
