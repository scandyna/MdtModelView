// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_NUMERIC_LIMITS_H
#define MDT_ITEM_MODEL_NUMERIC_LIMITS_H

#include <limits>

namespace Mdt{ namespace ItemModel{

  /*! \brief Get the largest positive value for int
   *
   * This is a commodity function using std::numeric_limits<int>::max()
   */
  inline
  constexpr
  int intMax() noexcept
  {
    return std::numeric_limits<int>::max();
  }

  /*! \brief Check if given value is the largest positive one for int
   *
   * \note Returns true only if \a i represents the positive largest value for int.
   */
  inline
  constexpr
  bool isIntMax(int i) noexcept
  {
    return i == intMax();
  }

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_NUMERIC_LIMITS_H
