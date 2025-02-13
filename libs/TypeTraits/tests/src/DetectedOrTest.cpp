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
#include <Mdt/TypeTraits/detected_or.h>
#include <type_traits>

namespace Impl{

  template<typename Container>
  using get_member_difference_type_op = typename Container::difference_type;

} // namespace Impl{

template<typename Container>
using get_member_difference_type_or_void = Mdt::TypeTraits::detected_or_t<void, Impl::get_member_difference_type_op, Container>;


struct NoDifferenceType
{
};

struct WithDifferenceType
{
  using difference_type = int;
};

template<typename Container>
struct Adapter
{
  // Will be Container::difference_type if it defines it, otherwise void
  using difference_type = get_member_difference_type_or_void<Container>;
};

using BasicContainer = Adapter<NoDifferenceType>;
using GoodContainer = Adapter<WithDifferenceType>;

static_assert( std::is_void_v<BasicContainer::difference_type> );
static_assert(std::is_same<GoodContainer::difference_type, int>::value);
