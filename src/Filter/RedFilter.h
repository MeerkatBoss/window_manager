/**
 * @file RedFilter.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-08
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __FILTER_RED_FILTER_H
#define __FILTER_RED_FILTER_H

#include "Filter/Filter.h"
namespace filter
{

class RedFilter : public Filter
{
public:
  virtual void applyFilter(gui::Canvas&      canvas,
                           const FilterMask& mask) override;
};

} // namespace filter

#endif /* RedFilter.h */
