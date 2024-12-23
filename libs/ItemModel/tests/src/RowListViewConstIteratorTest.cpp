// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "Mdt/ItemModel/RowListViewConstIterator.h"
#include "Mdt/ItemModel/RowRangeList.h"
#include "Mdt/ItemModel/RowRange.h"
#include <algorithm>
#include <vector>
#include <iterator>

using namespace Mdt::ItemModel;


TEST_CASE("basicRequirements")
{
  RowRangeList rangeList;
  rangeList.addRange( RowRange::fromFirstAndLastRow(0,1) );

  RowListViewConstIterator it( rangeList.cbegin() );
  RowListViewConstIterator endIt( rangeList.cend() );

  SECTION("iterator is dereferenceable")
  {
    REQUIRE( *it == 0 );
  }

  SECTION("iterator is incrementable (pre-increment)")
  {
    ++it;
    REQUIRE( *it == 1 );
  }

  SECTION("iterator is incrementable (post-increment)")
  {
    REQUIRE( *it++ == 0 );
  }

  SECTION("iterators can be compared")
  {
    REQUIRE( !(it == endIt) );
    REQUIRE( it != endIt );
  }
}

TEST_CASE("construct")
{
  RowRangeList rangeList;

  SECTION("construct an iterator to a empty range works (does not crash)")
  {
    RowListViewConstIterator it( rangeList.cbegin() );
    RowListViewConstIterator endIt( rangeList.cend() );

    REQUIRE( it == endIt );
  }

  SECTION("construct an iterator to the first range makes it dereferenceable")
  {
    rangeList.addRange( RowRange::fromFirstAndLastRow(0,1) );

    RowListViewConstIterator it( rangeList.cbegin() );

    REQUIRE( *it == 0 );
  }
}

TEST_CASE("iterate")
{
  RowRangeList rangeList;

  SECTION("single row range [0]")
  {
    rangeList.addRange( RowRange::fromFirstAndLastRow(0,0) );

    RowListViewConstIterator it( rangeList.cbegin() );
    RowListViewConstIterator endIt( rangeList.cend() );

    REQUIRE( it != endIt );
    REQUIRE( *it == 0 );

    ++it;
    REQUIRE( it == endIt );
  }

  SECTION("single row range [1]")
  {
    rangeList.addRange( RowRange::fromFirstAndLastRow(1,1) );

    RowListViewConstIterator it( rangeList.cbegin() );
    RowListViewConstIterator endIt( rangeList.cend() );

    REQUIRE( it != endIt );
    REQUIRE( *it == 1 );

    ++it;
    REQUIRE( it == endIt );
  }

  SECTION("single row range [1-2]")
  {
    rangeList.addRange( RowRange::fromFirstAndLastRow(1,2) );

    RowListViewConstIterator it( rangeList.cbegin() );
    RowListViewConstIterator endIt( rangeList.cend() );

    REQUIRE( it != endIt );
    REQUIRE( *it == 1 );

    ++it;
    REQUIRE( it != endIt );
    REQUIRE( *it == 2 );

    ++it;
    REQUIRE( it == endIt );
  }

  SECTION("2 row ranges [1-2],[4-6]")
  {
    rangeList.addRange( RowRange::fromFirstAndLastRow(1,2) );
    rangeList.addRange( RowRange::fromFirstAndLastRow(4,6) );

    RowListViewConstIterator it( rangeList.cbegin() );
    RowListViewConstIterator endIt( rangeList.cend() );

    REQUIRE( it != endIt );
    REQUIRE( *it == 1 );

    ++it;
    REQUIRE( it != endIt );
    REQUIRE( *it == 2 );

    ++it;
    REQUIRE( it != endIt );
    REQUIRE( *it == 4 );

    ++it;
    REQUIRE( it != endIt );
    REQUIRE( *it == 5 );

    ++it;
    REQUIRE( it != endIt );
    REQUIRE( *it == 6 );

    ++it;
    REQUIRE( it == endIt );
  }
}

// TEST_CASE("defaultConstruct")
// {
//   RowRangeList rangeList;
//   RowListViewConstIterator it;
//   RowListViewConstIterator endIt( rangeList.cend() );
//
//   REQUIRE( it == endIt );
// }

TEST_CASE("multipass_dereferenceManyTimes")
{
  RowRangeList rangeList;
  rangeList.addRange( RowRange::fromFirstAndLastRow(1,2) );
  rangeList.addRange( RowRange::fromFirstAndLastRow(4,6) );

  RowListViewConstIterator a( rangeList.cbegin() );
  RowListViewConstIterator endIt( rangeList.cend() );

  RowListViewConstIterator b = a;
  REQUIRE( a == b );
  REQUIRE( *a == 1 );
  REQUIRE( *b == 1 );
  REQUIRE( *a == *b );

  REQUIRE( *a == 1 );
  REQUIRE( *a == 1 );
  REQUIRE( *a == 1 );

  ++a;
  REQUIRE( a != b );
  REQUIRE( *a == 2 );
  REQUIRE( *b == 1 );

  ++b;
  REQUIRE( a == b );
  REQUIRE( *a == 2 );
  REQUIRE( *b == 2 );
  REQUIRE( *a == *b );

  REQUIRE( ++a == ++b );
}

TEST_CASE("multipass_traverseManyTimes")
{
  RowRangeList rangeList;
  rangeList.addRange( RowRange::fromFirstAndLastRow(1,2) );
  rangeList.addRange( RowRange::fromFirstAndLastRow(4,6) );

  RowListViewConstIterator it( rangeList.cbegin() );
  RowListViewConstIterator endIt( rangeList.cend() );

  /*
   * First pass
   */

  REQUIRE( it != endIt );
  REQUIRE( *it == 1 );

  ++it;
  REQUIRE( it != endIt );
  REQUIRE( *it == 2 );

  ++it;
  REQUIRE( it != endIt );
  REQUIRE( *it == 4 );

  ++it;
  REQUIRE( it != endIt );
  REQUIRE( *it == 5 );

  ++it;
  REQUIRE( it != endIt );
  REQUIRE( *it == 6 );

  ++it;
  REQUIRE( it == endIt );

  /*
   * Second pass
   */

  it = RowListViewConstIterator( rangeList.cbegin() );
  REQUIRE( it != endIt );
  REQUIRE( *it == 1 );

  ++it;
  REQUIRE( it != endIt );
  REQUIRE( *it == 2 );

  ++it;
  REQUIRE( it != endIt );
  REQUIRE( *it == 4 );

  ++it;
  REQUIRE( it != endIt );
  REQUIRE( *it == 5 );

  ++it;
  REQUIRE( it != endIt );
  REQUIRE( *it == 6 );

  ++it;
  REQUIRE( it == endIt );
}

TEST_CASE("std_copy")
{
  RowRangeList rangeList;
  rangeList.addRange( RowRange::fromFirstAndLastRow(0,1) );
  rangeList.addRange( RowRange::fromFirstAndLastRow(3,5) );

  RowListViewConstIterator first( rangeList.cbegin() );
  RowListViewConstIterator last( rangeList.cend() );

  std::vector<int> rowList;

  std::copy( first, last, std::back_inserter(rowList) );

  REQUIRE( rowList.size() == 5 );
  REQUIRE( rowList[0] == 0 );
  REQUIRE( rowList[1] == 1 );
  REQUIRE( rowList[2] == 3 );
  REQUIRE( rowList[3] == 4 );
  REQUIRE( rowList[4] == 5 );
}
