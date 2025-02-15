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
 ** https://en.cppreference.com/w/cpp/experimental/nonesuch
 *****************************************************************************************/
#ifndef MDT_TYPE_TRAITS_IMPL_NONESUCH_CUSTOM_H
#define MDT_TYPE_TRAITS_IMPL_NONESUCH_CUSTOM_H

namespace Mdt{ namespace TypeTraits{ namespace Impl{

  struct nonesuch {
    ~nonesuch() = delete;
    nonesuch(nonesuch const&) = delete;
    void operator=(nonesuch const&) = delete;
  };

}}} // namespace Mdt{ namespace TypeTraits{ namespace Impl{

#endif // #ifndef MDT_TYPE_TRAITS_IMPL_NONESUCH_CUSTOM_H
