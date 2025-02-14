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
#include "Mdt/TypeTraits/is_detected.h"
#include <utility>
#include <vector>
#include "catch2/catch.hpp"


struct Item
{
  int id = 0;
};

struct FixedSizeContainer
{
  using const_reference = const Item &;
};

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

  static_assert( !Mdt::TypeTraits::is_detected_v<has_insert_op, FixedSizeContainer> );
  static_assert( Mdt::TypeTraits::is_detected_v<has_insert_op, ContainerWithInsert> );

} // namespace Impl{


template<typename Container>
constexpr
bool containerSupportsInsert() noexcept
{
  return Mdt::TypeTraits::is_detected_v<Impl::has_insert_op, Container>;
}

static_assert( !containerSupportsInsert<FixedSizeContainer>() );
static_assert( containerSupportsInsert<ContainerWithInsert>() );


template<typename Container>
struct Adapter
{
  using const_reference = typename Container::const_reference;

  bool insert([[maybe_unused]] int index, [[maybe_unused]] int count, [[maybe_unused]] const_reference element)
  {
    if constexpr( containerSupportsInsert<Container>() ){
      // private helper to convert between iterator, size_type - int
      insertImpl(index, count, element);
      return true;
    }
    return false;
  }

  void insertImpl(int, int, const_reference)
  {
  }
};

Adapter<FixedSizeContainer> fixedSizeContainer;
Adapter<ContainerWithInsert> containerWithInsert;


TEST_CASE("insert")
{
  SECTION("fixed size container")
  {
    Adapter<FixedSizeContainer> fixedSizeContainer;

    CHECK( !fixedSizeContainer.insert( 0, 1, Item{} ) );
  }

  SECTION("container with insert")
  {
    Adapter<ContainerWithInsert> containerWithInsert;

    CHECK( containerWithInsert.insert( 0, 1, Item{} ) );
  }
}
