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
#ifndef MDT_TYPE_TRAITS_IMPL_DETECED_OR_CUSTOM_H
#define MDT_TYPE_TRAITS_IMPL_DETECED_OR_CUSTOM_H

#include "Mdt/TypeTraits/Impl/detector_custom.h"

namespace Mdt{ namespace TypeTraits{ namespace Impl{

  template<template<class...> class Op, class... Args>
  using detected_t = typename detector<nonesuch, void, Op, Args...>::type;

  template<class Default, template<class...> class Op, class... Args>
  using detected_or = detector<Default, void, Op, Args...>;

  template< class Default, template<class...> class Op, class... Args >
  using detected_or_t = typename detected_or<Default, Op, Args...>::type;

}}} // namespace Mdt{ namespace TypeTraits{ namespace Impl{

#endif // #ifndef MDT_TYPE_TRAITS_IMPL_DETECED_OR_CUSTOM_H
