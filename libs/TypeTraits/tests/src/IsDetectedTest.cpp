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
#include "Mdt/TypeTraits/detected_or.h"
#include "Mdt/TypeTraits/is_detected.h"
#include <utility>
#include <vector>
// #include "catch2/catch.hpp"

struct Item
{
  int id = 0;
};

struct NoInsert
{
};

// struct WithInsert
// {
//   void insert(int count, bool b);
// };

struct ContainerWithInsert
{
  using size_type = std::vector<Item>::size_type;
  using const_reference = const Item &;
  using const_iterator = std::vector<Item>::const_iterator;

  void insert(const_iterator pos, size_type count, const Item & item);
};


namespace Impl{

  template<typename Container>
  using has_insert_op = decltype(
    std::declval<Container>().insert( std::declval<typename Container::const_iterator>(),
                                      std::declval<typename Container::size_type>(),
                                      std::declval<typename Container::const_reference>() )
  );

  static_assert( !Mdt::TypeTraits::is_detected_v<has_insert_op, NoInsert> );
  static_assert( Mdt::TypeTraits::is_detected_v<has_insert_op, ContainerWithInsert> );

} // namespace Impl{


template<typename Container>
constexpr
bool containerSupportsInsert() noexcept
{
  return Mdt::TypeTraits::is_detected_v<Impl::has_insert_op, Container>;
}

static_assert( !containerSupportsInsert<NoInsert>() );
static_assert( containerSupportsInsert<ContainerWithInsert>() );


/** detected_or
 */

struct NoDifferenceType
{
};

struct WithDifferenceType
{
  using difference_type = int;
};

namespace Impl{

  /*! \internal
   */
  template<typename Container>
  using get_member_difference_type_op = typename Container::difference_type;

  // template<typename Container, typename MemberType>
  // using get_member_type_op = typename Container::MemberType;

} // namespace Impl{

/*! \brief
 */
template<typename Container>
using get_member_difference_type_or_void = Mdt::TypeTraits::detected_or_t<void, Impl::get_member_difference_type_op, Container>;




template<typename Container>
struct Adapter
{
  // Will be Container::difference_type if it defines it, otherwise void
  using difference_type = get_member_difference_type_or_void<Container>;
  // using difference_type = Mdt::detected_or_t<void, get_difference_type_op, Container>;
  // using difference_type = difference_type<Container>;

  bool insert()
  {
    if constexpr( containerSupportsInsert<Container>() ){
    }
    return false;
  }
};

using BasicContainer = Adapter<NoDifferenceType>;
using GoodContainer = Adapter<WithDifferenceType>;

static_assert(std::is_void_v<BasicContainer::difference_type>);
static_assert(std::is_same<GoodContainer::difference_type, int>::value);


// Example from Clow

struct Yes
{
  using size_type = unsigned int;
};

struct No
{
};

template<typename T>
using has_size = typename T::size_type;

template<typename T>
void doSomething(const T & t)
{
  Mdt::TypeTraits::detected_or<short, has_size, T> v;
}
