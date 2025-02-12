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
#ifndef MDT_TYPE_TRAITS_DETAIL_DETECTOR_H
#define MDT_TYPE_TRAITS_DETAIL_DETECTOR_H

#ifdef MDT_TYPE_TRAITS_COMPILER_SUPPORTS_DETECTION_IDIOM
 #include "Mdt/TypeTraits/detail/detector_std.h"
#else
 #include "Mdt/TypeTraits/detail/detector_custom.h"
#endif // #ifdef MDT_TYPE_TRAITS_COMPILER_SUPPORTS_DETECTION_IDIOM

namespace Mdt{ namespace TypeTraits{ namespace Impl{

  

}}} // namespace Mdt{ namespace TypeTraits{ namespace Impl{

#endif // #ifndef MDT_TYPE_TRAITS_DETAIL_DETECTOR_H
