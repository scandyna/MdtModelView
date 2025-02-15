// SPDX-License-Identifier: BSL-1.0
/****************************************************************************************
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 ** Distributed under the Boost Software License,
 ** Version 1.0. (See accompanying file LICENSE_1_0.txt
 ** or copy at http://www.boost.org/LICENSE_1_0.txt)
 **
 *****************************************************************************************
 ** This is almost a copy paste from
 ** https://en.cppreference.com/w/cpp/experimental/is_detected
 *****************************************************************************************/
#ifndef MDT_TYPE_TRAITS_IMPL_IS_DETECED_CUSTOM_H
#define MDT_TYPE_TRAITS_IMPL_IS_DETECED_CUSTOM_H

#include "Mdt/TypeTraits/Impl/detector_custom.h"

namespace Mdt{ namespace TypeTraits{ namespace Impl{

  template<template<class...> class Op, class... Args>
  using is_detected = typename detector<nonesuch, void, Op, Args...>::value_t;

  template<template<class...> class Op, class... Args>
  using detected_t = typename detector<nonesuch, void, Op, Args...>::type;

  template< template<class...> class Op, class... Args >
  constexpr inline bool is_detected_v = is_detected<Op, Args...>::value;

}}} // namespace Mdt{ namespace TypeTraits{ namespace Impl{

#endif // #ifndef MDT_TYPE_TRAITS_IMPL_IS_DETECED_CUSTOM_H
