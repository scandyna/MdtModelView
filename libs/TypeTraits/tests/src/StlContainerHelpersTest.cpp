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
#include "Mdt/TypeTraits/StlContainerHelpers.h"
#include "catch2/catch.hpp"
#include <vector>
#include <type_traits>


/*
 * Types used for tests
 */

struct EmptyStruct
{
};

using Vector = std::vector<int>;


/*
 * Some helper functions static tests
 */

static_assert( Mdt::TypeTraits::is_void_or_void_pointer<void>() );
static_assert( Mdt::TypeTraits::is_void_or_void_pointer<void*>() );

static_assert( !Mdt::TypeTraits::is_void_or_void_pointer<int>() );
static_assert( !Mdt::TypeTraits::is_void_or_void_pointer<int*>() );


/*
 * Member types static tests
 */

static_assert( std::is_void_v< Mdt::TypeTraits::member_value_type_or_void<EmptyStruct> >);
static_assert( std::is_same_v< Mdt::TypeTraits::member_value_type_or_void<Vector>, Vector::value_type >);

static_assert( std::is_void_v< Mdt::TypeTraits::member_size_type_or_void<EmptyStruct> >);
static_assert( std::is_same_v< Mdt::TypeTraits::member_size_type_or_void<Vector>, Vector::size_type >);

static_assert( std::is_void_v< Mdt::TypeTraits::member_difference_type_or_void<EmptyStruct> >);
static_assert( std::is_same_v< Mdt::TypeTraits::member_difference_type_or_void<Vector>, Vector::difference_type >);

static_assert( std::is_void_v< Mdt::TypeTraits::member_reference_or_void<EmptyStruct> >);
static_assert( std::is_same_v< Mdt::TypeTraits::member_reference_or_void<Vector>, Vector::reference >);

static_assert( std::is_void_v< Mdt::TypeTraits::member_const_reference_or_void<EmptyStruct> >);
static_assert( std::is_same_v< Mdt::TypeTraits::member_const_reference_or_void<Vector>, Vector::const_reference >);

static_assert( std::is_void_v< Mdt::TypeTraits::member_pointer_or_void<EmptyStruct> >);
static_assert( std::is_same_v< Mdt::TypeTraits::member_pointer_or_void<Vector>, Vector::pointer >);

static_assert( std::is_void_v< Mdt::TypeTraits::member_const_pointer_or_void<EmptyStruct> >);
static_assert( std::is_same_v< Mdt::TypeTraits::member_const_pointer_or_void<Vector>, Vector::const_pointer >);

static_assert( std::is_void_v< Mdt::TypeTraits::member_iterator_or_void<EmptyStruct> >);
static_assert( std::is_same_v< Mdt::TypeTraits::member_iterator_or_void<Vector>, Vector::iterator >);

static_assert( Mdt::TypeTraits::is_void_or_void_pointer< Mdt::TypeTraits::member_iterator_or_void_pointer<EmptyStruct> >() );
static_assert( std::is_same_v< Mdt::TypeTraits::member_iterator_or_void_pointer<EmptyStruct>, void* > );
static_assert( !Mdt::TypeTraits::is_void_or_void_pointer< Mdt::TypeTraits::member_iterator_or_void_pointer<Vector> >() );
static_assert( std::is_same_v< Mdt::TypeTraits::member_iterator_or_void_pointer<Vector>, Vector::iterator > );

static_assert( std::is_void_v< Mdt::TypeTraits::member_const_iterator_or_void<EmptyStruct> >);
static_assert( std::is_same_v< Mdt::TypeTraits::member_const_iterator_or_void<Vector>, Vector::const_iterator >);

static_assert( Mdt::TypeTraits::is_void_or_void_pointer< Mdt::TypeTraits::member_const_iterator_or_void_pointer<EmptyStruct> >() );
static_assert( std::is_same_v< Mdt::TypeTraits::member_const_iterator_or_void_pointer<EmptyStruct>, void* > );
static_assert( !Mdt::TypeTraits::is_void_or_void_pointer< Mdt::TypeTraits::member_const_iterator_or_void_pointer<Vector> >() );
static_assert( std::is_same_v< Mdt::TypeTraits::member_const_iterator_or_void_pointer<Vector>, Vector::const_iterator >);

static_assert( std::is_void_v< Mdt::TypeTraits::member_reverse_iterator_or_void<EmptyStruct> >);
static_assert( std::is_same_v< Mdt::TypeTraits::member_reverse_iterator_or_void<Vector>, Vector::reverse_iterator >);

static_assert( std::is_void_v< Mdt::TypeTraits::member_const_reverse_iterator_or_void<EmptyStruct> >);
static_assert( std::is_same_v< Mdt::TypeTraits::member_const_reverse_iterator_or_void<Vector>, Vector::const_reverse_iterator >);

/*
 * Member functions static tests
 */

static_assert( !Mdt::TypeTraits::has_member_insert_pos_count_value<EmptyStruct>() );
static_assert( Mdt::TypeTraits::has_member_insert_pos_count_value<Vector>() );

static_assert( !Mdt::TypeTraits::has_member_erase_first_last<EmptyStruct>() );
static_assert( Mdt::TypeTraits::has_member_erase_first_last<Vector>() );

/*
 * Adapter example
 */

template<typename Container>
struct Adapter
{
  // size_type is mandatory
  using size_type = typename Container::size_type;

  // const_reference is mandatory
  using const_reference = typename Container::const_reference;

  // Will be Container::const_iterator if it defines it, otherwise void
  using const_iterator = Mdt::TypeTraits::member_const_iterator_or_void<Container>;

  bool insert([[maybe_unused]] int index, [[maybe_unused]] int count, [[maybe_unused]] const_reference element)
  {
    if constexpr( Mdt::TypeTraits::has_member_insert_pos_count_value<Container>() ){
      // private helper to convert between iterator, size_type - int
      insertImpl(index, count, element);
      return true;
    }
    return false;
  }

  bool removeRows([[maybe_unused]] int row, [[maybe_unused]] int count)
  {
    if constexpr( Mdt::TypeTraits::has_member_erase_first_last<Container>() ){
      eraseImpl(row, count);
      return true;
    }
    return false;
  }

  void insertImpl(int, int, const_reference)
  {
  }

  void eraseImpl(int, int)
  {
  }
};

struct Item
{
  int id = 0;
};

struct FixedSizeContainer
{
  using size_type = std::vector<Item>::size_type;
  using const_reference = const Item &;
};

struct ContainerWithInsert
{
  using size_type = std::vector<Item>::size_type;
  using const_reference = const Item &;
  using const_iterator = std::vector<Item>::const_iterator;

  void insert(const_iterator pos, size_type count, const Item & item);
};

struct ContainerWithErase
{
  using size_type = std::vector<Item>::size_type;
  using const_reference = const Item &;
  using const_iterator = std::vector<Item>::const_iterator;

  void erase(const_iterator first, const_iterator last);
};

struct AdapterFunctionMapWithErase
{
  using size_type = ContainerWithErase::size_type;
  using const_reference = const Item &;
  using const_iterator = ContainerWithErase::const_iterator;

  static
  void erase(ContainerWithErase & list, const_iterator first, const_iterator last);
};


struct ContainerWithMaxSize
{
  using size_type = std::vector<Item>::size_type;

  size_type max_size() const noexcept
  {
    return 27;
  }
};

struct AdapterFunctionMapWithMaxSize
{
  using size_type = ContainerWithMaxSize::size_type;

  static
  size_type maxSize(const ContainerWithMaxSize & list) noexcept
  {
    return list.max_size();
  }
};


/*
 * maxSize() member functions static tests
 */

static_assert( !Mdt::TypeTraits::has_member_maxSize_container<AdapterFunctionMapWithErase, ContainerWithErase>() );
static_assert( Mdt::TypeTraits::has_member_maxSize_container<AdapterFunctionMapWithMaxSize, ContainerWithMaxSize>() );


/*
 * Runtime tests
 */

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

TEST_CASE("removeRows")
{
  SECTION("fixed size container")
  {
    Adapter<FixedSizeContainer> fixedSizeContainer;

    CHECK( !fixedSizeContainer.removeRows(0, 1) );
  }

  SECTION("container with erase")
  {
    Adapter<ContainerWithErase> containerWithErase;

    CHECK( containerWithErase.removeRows(0, 1) );
  }
}
