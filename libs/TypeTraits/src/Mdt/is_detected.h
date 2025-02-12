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
#ifndef MDT_TYPE_TRAITS_IS_DETECED_H
#define MDT_TYPE_TRAITS_IS_DETECED_H

#ifdef MDT_TYPE_TRAITS_COMPILER_SUPPORTS_DETECTION_IDIOM
 #include "Mdt/TypeTraits/detail/is_detected_std.h"
#else
 #include "Mdt/TypeTraits/detail/is_detected_custom.h"
#endif // #ifdef MDT_TYPE_TRAITS_COMPILER_SUPPORTS_DETECTION_IDIOM

namespace Mdt{

  

} // namespace Mdt{

#endif // #ifndef MDT_TYPE_TRAITS_IS_DETECED_H
