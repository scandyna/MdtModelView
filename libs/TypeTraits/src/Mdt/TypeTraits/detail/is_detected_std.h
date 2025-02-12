// SPDX-License-Identifier: BSL-1.0
/****************************************************************************************
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 ** Distributed under the Boost Software License,
 ** Version 1.0. (See accompanying file LICENSE_1_0.txt
 ** or copy at http://www.boost.org/LICENSE_1_0.txt)
 **
 *****************************************************************************************/
#ifndef MDT_TYPE_TRAITS_DETAIL_IS_DETECED_STD_H
#define MDT_TYPE_TRAITS_DETAIL_IS_DETECED_STD_H

#include <experimental/type_traits>

namespace Mdt{

  using std::experimental::is_detected;
  using std::experimental::detected_t;
  using std::experimental::is_detected_v;

} // namespace Mdt{

#endif // #ifndef MDT_TYPE_TRAITS_DETAIL_IS_DETECED_STD_H
