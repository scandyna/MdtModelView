// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_NUMERIC_LIMITS_H
#define MDT_ITEM_MODEL_NUMERIC_LIMITS_H

#include <Mdt/Numeric/Limits.h>
#include <Mdt/Numeric/BasicConversion.h>
#include <Mdt/Numeric/IntegerComparison.h>
#include <limits>
#include <type_traits>
#include <cassert>

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

  /*! \brief Returns the minimum value between given \a value and int max
   *
   * \pre \a value must be an integral type
   */
  template<typename SizeType>
  constexpr
  int minBetweenSizeTypeValueAndIntMax(SizeType value) noexcept
  {
    static_assert(std::is_integral_v<SizeType>, "Mdt::ItemModel::minBetweenSizeTypeValueAndIntMax(): given value must be an integral type");
    static_assert(!std::is_same_v<SizeType, bool>, "Mdt::ItemModel::minBetweenSizeTypeValueAndIntMax(): given value must be an integral type (not bool)");
    static_assert(!std::is_same_v<SizeType, char>, "Mdt::ItemModel::minBetweenSizeTypeValueAndIntMax(): given value must be an integral type (not char)");

    if( Mdt::Numeric::cmp_less( value, intMax() ) ){
      if( Mdt::Numeric::int_canHoldValueOf_T(value) ){
        return Mdt::Numeric::int_from_T(value);
      }
      return std::numeric_limits<int>::min();
    }

    return intMax();
  }

  /*! \brief Returns the minimum value between \a SizeType max and int max
   *
   * \pre \a SizeType must be an integral type
   */
  template<typename SizeType>
  constexpr
  int minBetweenSizeTypeMaxAndIntMax() noexcept
  {
    static_assert(std::is_integral_v<SizeType>, "Mdt::ItemModel::minBetweenSizeTypeMaxAndIntMax(): SizeType must be an integral type");
    static_assert(!std::is_same_v<SizeType, bool>, "Mdt::ItemModel::minBetweenSizeTypeMaxAndIntMax(): SizeType must be an integral type (not bool)");
    static_assert(!std::is_same_v<SizeType, char>, "Mdt::ItemModel::minBetweenSizeTypeMaxAndIntMax(): SizeType must be an integral type (not char)");

    return minBetweenSizeTypeValueAndIntMax( std::numeric_limits<SizeType>::max() );
  }

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_NUMERIC_LIMITS_H
