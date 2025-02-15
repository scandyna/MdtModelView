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
#ifndef MDT_TYPE_TRAITS_IMPL_DETECTOR_CUSTOM_H
#define MDT_TYPE_TRAITS_IMPL_DETECTOR_CUSTOM_H

#include "Mdt/TypeTraits/Impl/nonesuch_custom.h"
#include <type_traits>

namespace Mdt{ namespace TypeTraits{ namespace Impl{

  template<class Default, class AlwaysVoid, template<class...> class Op, class... Args>
  struct detector
  {
    using value_t = std::false_type;
    using type = Default;
  };

  template<class Default, template<class...> class Op, class... Args>
  struct detector<Default, std::void_t<Op<Args...>>, Op, Args...>
  {
    using value_t = std::true_type;
    using type = Op<Args...>;
  };

}}} // namespace Mdt{ namespace TypeTraits{ namespace Impl{

#endif // #ifndef MDT_TYPE_TRAITS_IMPL_DETECTOR_CUSTOM_H
