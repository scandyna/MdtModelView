// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "Mdt/ItemModel/RowRangeListAlgorithm.h"
#include "Mdt/ItemModel/NumericLimits.h"

using namespace Mdt::ItemModel;


TEST_CASE("rangeAcomesBeforeB")
{
  SECTION("Disjoint ranges")
  {
    SECTION("[0,0] comes before [1,1]")
    {
      auto a = RowRange::fromFirstAndLastRow(0, 0);
      auto b = RowRange::fromFirstAndLastRow(1, 1);

      REQUIRE( rangeAcomesBeforeB(a, b) );
    }

    SECTION("a [1,1] comes not before b [0,0]")
    {
      auto a = RowRange::fromFirstAndLastRow(1, 1);
      auto b = RowRange::fromFirstAndLastRow(0, 0);

      REQUIRE( !rangeAcomesBeforeB(a, b) );
    }
  }

  SECTION("overlapping ranges where a begins before b")
  {
    SECTION("[0,1] comes before [1,1]")
    {
      auto a = RowRange::fromFirstAndLastRow(0, 1);
      auto b = RowRange::fromFirstAndLastRow(1, 1);

      REQUIRE( rangeAcomesBeforeB(a, b) );
    }

    SECTION("[0,1] comes before [1,4]")
    {
      auto a = RowRange::fromFirstAndLastRow(0, 1);
      auto b = RowRange::fromFirstAndLastRow(1, 4);

      REQUIRE( rangeAcomesBeforeB(a, b) );
    }
  }

  SECTION("overlapping ranges, with equal beginnings, where a ends before b")
  {
    SECTION("a [0,1] comes before b [0,2]")
    {
      auto a = RowRange::fromFirstAndLastRow(0, 1);
      auto b = RowRange::fromFirstAndLastRow(0, 2);

      REQUIRE( rangeAcomesBeforeB(a, b) );
    }
  }

  SECTION("overlapping ranges, with equal beginnings, where a not ends before b")
  {
    SECTION("a [0,2] comes not before b [0,1]")
    {
      auto a = RowRange::fromFirstAndLastRow(0, 2);
      auto b = RowRange::fromFirstAndLastRow(0, 1);

      REQUIRE( !rangeAcomesBeforeB(a, b) );
    }
  }

  SECTION("equal ranges")
  {
    SECTION("a [0,1] comes not before b [0,1]")
    {
      auto a = RowRange::fromFirstAndLastRow(0, 1);
      auto b = RowRange::fromFirstAndLastRow(0, 1);

      REQUIRE( !rangeAcomesBeforeB(a, b) );
    }
  }
}

TEST_CASE("isSorted")
{
  RowRangeListContainer list;

  SECTION("empty")
  {
    REQUIRE( isSorted(list) );
  }

  SECTION("{[0,1],[2,3]} is sorted")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );
    list.emplace_back( RowRange::fromFirstAndLastRow(2, 3) );

    REQUIRE( isSorted(list) );
  }

  SECTION("{[2,3],[0,1]} is not sorted")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(2, 3) );
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );

    REQUIRE( !isSorted(list) );
  }
}

TEST_CASE("rangesAreAdjacent")
{
  SECTION("[0,1] [2,3] are adjacent")
  {
    auto a = RowRange::fromFirstAndLastRow(0, 1);
    auto b = RowRange::fromFirstAndLastRow(2, 3);

    REQUIRE( rangesAreAdjacent(a, b) );
  }

  SECTION("[2,3] [0,1] are adjacent")
  {
    auto a = RowRange::fromFirstAndLastRow(2, 3);
    auto b = RowRange::fromFirstAndLastRow(0, 1);

    REQUIRE( rangesAreAdjacent(a, b) );
  }

  SECTION("[0,1] [3,4] are not adjacent")
  {
    auto a = RowRange::fromFirstAndLastRow(0, 1);
    auto b = RowRange::fromFirstAndLastRow(3, 4);

    REQUIRE( !rangesAreAdjacent(a, b) );
  }

  SECTION("[3,4] [0,1] are not adjacent")
  {
    auto a = RowRange::fromFirstAndLastRow(3, 4);
    auto b = RowRange::fromFirstAndLastRow(0, 1);

    REQUIRE( !rangesAreAdjacent(a, b) );
  }

  SECTION("[0,1] [1,2] are not adjacent")
  {
    auto a = RowRange::fromFirstAndLastRow(0, 1);
    auto b = RowRange::fromFirstAndLastRow(1, 2);

    REQUIRE( !rangesAreAdjacent(a, b) );
  }

  SECTION("[1,2] [0,1] are not adjacent")
  {
    auto a = RowRange::fromFirstAndLastRow(1, 2);
    auto b = RowRange::fromFirstAndLastRow(0, 1);

    REQUIRE( !rangesAreAdjacent(a, b) );
  }

  SECTION("[100,INT_MAX] [1000,1050] are not adjacent")
  {
    auto a = RowRange::fromFirstAndLastRow( 100, intMax() );
    auto b = RowRange::fromFirstAndLastRow(1000, 1050);

    REQUIRE( !rangesAreAdjacent(a, b) );
  }

  SECTION("[1000,1050] [100,INT_MAX] are not adjacent")
  {
    auto a = RowRange::fromFirstAndLastRow(1000, 1050);
    auto b = RowRange::fromFirstAndLastRow( 100, intMax() );

    REQUIRE( !rangesAreAdjacent(a, b) );
  }
}

TEST_CASE("rangesOverlaps")
{
  SECTION("[0,1] [1,2] overlaps")
  {
    auto a = RowRange::fromFirstAndLastRow(0, 1);
    auto b = RowRange::fromFirstAndLastRow(1, 2);

    REQUIRE( rangesOverlaps(a, b) );
  }

  SECTION("[1,2] [0,1] overlaps")
  {
    auto a = RowRange::fromFirstAndLastRow(1, 2);
    auto b = RowRange::fromFirstAndLastRow(0, 1);

    REQUIRE( rangesOverlaps(a, b) );
  }

  SECTION("[0,1] [0,2] overlaps")
  {
    auto a = RowRange::fromFirstAndLastRow(0, 1);
    auto b = RowRange::fromFirstAndLastRow(0, 2);

    REQUIRE( rangesOverlaps(a, b) );
  }

  SECTION("[0,2] [0,1] overlaps")
  {
    auto a = RowRange::fromFirstAndLastRow(0, 2);
    auto b = RowRange::fromFirstAndLastRow(0, 1);

    REQUIRE( rangesOverlaps(a, b) );
  }

  SECTION("[0,1] [2,3] do not overlaps")
  {
    auto a = RowRange::fromFirstAndLastRow(0, 1);
    auto b = RowRange::fromFirstAndLastRow(2, 3);

    REQUIRE( !rangesOverlaps(a, b) );
  }

  SECTION("[2,3] [0,1] do not overlaps")
  {
    auto a = RowRange::fromFirstAndLastRow(2, 3);
    auto b = RowRange::fromFirstAndLastRow(0, 1);

    REQUIRE( !rangesOverlaps(a, b) );
  }
}

TEST_CASE("rangesShouldBeMerged")
{
  SECTION("adjacent ranges should be merged")
  {
    SECTION("[0,1] [2,3]")
    {
      auto a = RowRange::fromFirstAndLastRow(0, 1);
      auto b = RowRange::fromFirstAndLastRow(2, 3);

      REQUIRE( rangesShouldBeMerged(a, b) );
    }

    SECTION("[2,3] [0,1]")
    {
      auto a = RowRange::fromFirstAndLastRow(2, 3);
      auto b = RowRange::fromFirstAndLastRow(0, 1);

      REQUIRE( rangesShouldBeMerged(a, b) );
    }
  }

  SECTION("overlapping ranges should be merged")
  {
    SECTION("[0,1] [1,3]")
    {
      auto a = RowRange::fromFirstAndLastRow(0, 1);
      auto b = RowRange::fromFirstAndLastRow(1, 3);

      REQUIRE( rangesShouldBeMerged(a, b) );
    }

    SECTION("[1,3] [0,1]")
    {
      auto a = RowRange::fromFirstAndLastRow(1, 3);
      auto b = RowRange::fromFirstAndLastRow(0, 1);

      REQUIRE( rangesShouldBeMerged(a, b) );
    }

    SECTION("[0,1] [0,3]")
    {
      auto a = RowRange::fromFirstAndLastRow(0, 1);
      auto b = RowRange::fromFirstAndLastRow(0, 3);

      REQUIRE( rangesShouldBeMerged(a, b) );
    }

    SECTION("[0,3] [0,1]")
    {
      auto a = RowRange::fromFirstAndLastRow(0, 3);
      auto b = RowRange::fromFirstAndLastRow(0, 1);

      REQUIRE( rangesShouldBeMerged(a, b) );
    }

    SECTION("[2,3] [0,5]")
    {
      auto a = RowRange::fromFirstAndLastRow(2, 3);
      auto b = RowRange::fromFirstAndLastRow(0, 5);

      REQUIRE( rangesShouldBeMerged(a, b) );
    }
  }

  SECTION("non adjacent (nor non overlapping) ranges should not be merged")
  {
    SECTION("[0,1] [3,4]")
    {
      auto a = RowRange::fromFirstAndLastRow(0, 1);
      auto b = RowRange::fromFirstAndLastRow(3, 4);

      REQUIRE( !rangesShouldBeMerged(a, b) );
    }

    SECTION("[3,4] [0,1]")
    {
      auto a = RowRange::fromFirstAndLastRow(3, 4);
      auto b = RowRange::fromFirstAndLastRow(0, 1);

      REQUIRE( !rangesShouldBeMerged(a, b) );
    }
  }
}

TEST_CASE("mergeRanges")
{
  SECTION("[0,1] [2,3] gives [0,3]")
  {
    auto a = RowRange::fromFirstAndLastRow(0, 1);
    auto b = RowRange::fromFirstAndLastRow(2, 3);

    RowRange result = mergeRanges(a, b);

    REQUIRE( result.firstRow() == 0 );
    REQUIRE( result.lastRow() == 3 );
  }

  SECTION("[2,3] [0,1] gives [0,3]")
  {
    auto a = RowRange::fromFirstAndLastRow(2, 3);
    auto b = RowRange::fromFirstAndLastRow(0, 1);

    RowRange result = mergeRanges(a, b);

    REQUIRE( result.firstRow() == 0 );
    REQUIRE( result.lastRow() == 3 );
  }

  SECTION("[2,3] [0,5] gives [0,5]")
  {
    auto a = RowRange::fromFirstAndLastRow(2, 3);
    auto b = RowRange::fromFirstAndLastRow(0, 5);

    RowRange result = mergeRanges(a, b);

    REQUIRE( result.firstRow() == 0 );
    REQUIRE( result.lastRow() == 5 );
  }
}

TEST_CASE("mergeRanges_3")
{
  SECTION("[0,1] [2,3] [4,5] gives [0,5]")
  {
    auto a = RowRange::fromFirstAndLastRow(0, 1);
    auto b = RowRange::fromFirstAndLastRow(2, 3);
    auto c = RowRange::fromFirstAndLastRow(4, 5);

    RowRange result = mergeRanges(a, b, c);

    REQUIRE( result.firstRow() == 0 );
    REQUIRE( result.lastRow() == 5 );
  }

  SECTION("[2,3] [0,1] [4,5] gives [0,5]")
  {
    auto a = RowRange::fromFirstAndLastRow(2, 3);
    auto b = RowRange::fromFirstAndLastRow(0, 1);
    auto c = RowRange::fromFirstAndLastRow(4, 5);

    RowRange result = mergeRanges(a, b, c);

    REQUIRE( result.firstRow() == 0 );
    REQUIRE( result.lastRow() == 5 );
  }

  SECTION("[4,5] [0,1] [2,3] gives [0,5]")
  {
    auto a = RowRange::fromFirstAndLastRow(4, 5);
    auto b = RowRange::fromFirstAndLastRow(0, 1);
    auto c = RowRange::fromFirstAndLastRow(2, 3);

    RowRange result = mergeRanges(a, b, c);

    REQUIRE( result.firstRow() == 0 );
    REQUIRE( result.lastRow() == 5 );
  }
}

TEST_CASE("elementsAreNotMergeable")
{
  RowRangeListContainer list;

  SECTION("no element can be merged in an empty list")
  {
    REQUIRE( elementsAreNotMergeable(list) );
  }

  SECTION("{[0,1]} none mergeable")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );

    REQUIRE( elementsAreNotMergeable(list) );
  }

  SECTION("{[0,1],[3,4]} none mergeable")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );
    list.emplace_back( RowRange::fromFirstAndLastRow(3, 4) );

    REQUIRE( elementsAreNotMergeable(list) );
  }

  SECTION("{[0,1],[2,4]} mergeable")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );
    list.emplace_back( RowRange::fromFirstAndLastRow(2, 4) );

    REQUIRE( !elementsAreNotMergeable(list) );
  }

  SECTION("{[0,1],[1,4]} mergeable")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );
    list.emplace_back( RowRange::fromFirstAndLastRow(1, 4) );

    REQUIRE( !elementsAreNotMergeable(list) );
  }
}

TEST_CASE("findPotentialInsertionPoint")
{
  RowRangeListContainer list;

  SECTION("the point of an empty list is always past the end")
  {
    auto range = RowRange::fromFirstAndLastRow(0, 1);

    REQUIRE( findPotentialInsertionPoint(list, range) == list.cend() );
  }

  SECTION("given 1 element list {[0,1]}")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );

    SECTION("point for [3,4] is past the end")
    {
      auto range = RowRange::fromFirstAndLastRow(3, 4);

      REQUIRE( findPotentialInsertionPoint(list, range) == list.cend() );
    }

    SECTION("point for [0,1] is past the end")
    {
      auto range = RowRange::fromFirstAndLastRow(0, 1);

      REQUIRE( findPotentialInsertionPoint(list, range) == list.cend() );
    }
  }

  SECTION("given 1 element list {[3,4]}")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(3, 4) );

    SECTION("point for [0,1] is the first element")
    {
      auto range = RowRange::fromFirstAndLastRow(0, 1);

      REQUIRE( findPotentialInsertionPoint(list, range) == list.cbegin() );
    }
  }

  SECTION("point for [3,4] is at second el. in {[0,1],[6,8]}")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );
    list.emplace_back( RowRange::fromFirstAndLastRow(6, 8) );

    auto range = RowRange::fromFirstAndLastRow(3, 4);

    REQUIRE( findPotentialInsertionPoint(list, range) == (list.cbegin() + 1) );
  }

  SECTION("point for [2,4] is at second el. in {[0,1],[6,8]}")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );
    list.emplace_back( RowRange::fromFirstAndLastRow(6, 8) );

    auto range = RowRange::fromFirstAndLastRow(2, 4);

    REQUIRE( findPotentialInsertionPoint(list, range) == (list.cbegin() + 1) );
  }

  SECTION("point for [0,4] is at second el. in {[0,1],[6,8]}")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );
    list.emplace_back( RowRange::fromFirstAndLastRow(6, 8) );

    auto range = RowRange::fromFirstAndLastRow(0, 4);

    REQUIRE( findPotentialInsertionPoint(list, range) == (list.cbegin() + 1) );
  }

  SECTION("point for [0,0] at first el. in {[3,3]}")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(3, 3) );

    auto range = RowRange::fromFirstAndLastRow(0, 0);

    REQUIRE( findPotentialInsertionPoint(list, range) == list.cbegin() );
  }
}

TEST_CASE("findFirstElementToMerge")
{
  RowRangeListContainer list;

  SECTION("given single element list {[0,1]}")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );

    SECTION("insert point is first")
    {
      auto insertPoint = list.begin();

      SECTION("range [0,1] returns first")
      {
        auto range = RowRange::fromFirstAndLastRow(0, 1);

        auto it = findFirstElementToMerge(list.begin(), insertPoint, list.end(), range);

        REQUIRE( it == list.begin() );
      }

      SECTION("range [3,4] returns end")
      {
        auto range = RowRange::fromFirstAndLastRow(3, 4);

        auto it = findFirstElementToMerge(list.begin(), insertPoint, list.end(), range);

        REQUIRE( it == list.end() );
      }
    }

    SECTION("insert point is end")
    {
      auto insertPoint = list.end();

      SECTION("range [1,2] returns first")
      {
        auto range = RowRange::fromFirstAndLastRow(1, 2);

        auto it = findFirstElementToMerge(list.begin(), insertPoint, list.end(), range);

        REQUIRE( it == list.begin() );
      }

      SECTION("range [3,4] returns end")
      {
        auto range = RowRange::fromFirstAndLastRow(3, 4);

        auto it = findFirstElementToMerge(list.begin(), insertPoint, list.end(), range);

        REQUIRE( it == list.end() );
      }
    }
  }

  SECTION("given list {[0,1],[3,4]}")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );
    list.emplace_back( RowRange::fromFirstAndLastRow(3, 4) );

    SECTION("for [0,0] insert point is first, result is first")
    {
      auto range = RowRange::fromFirstAndLastRow(0, 0);
      auto insertPoint = list.begin();

      auto it = findFirstElementToMerge(list.begin(), insertPoint, list.end(), range);

      REQUIRE( it == list.begin() );
    }

    SECTION("for [1,2] insert point is second, result is first")
    {
      auto range = RowRange::fromFirstAndLastRow(1, 2);
      auto insertPoint = list.begin() + 1;

      auto it = findFirstElementToMerge(list.begin(), insertPoint, list.end(), range);

      REQUIRE( it == list.begin() );
    }

    SECTION("for [4,5] insert point is end, result is second")
    {
      auto range = RowRange::fromFirstAndLastRow(4, 5);
      auto insertPoint = list.end();

      auto it = findFirstElementToMerge(list.begin(), insertPoint, list.end(), range);

      REQUIRE( it == (list.begin() + 1) );
    }

    SECTION("for [6,7] insert point is end, result is end")
    {
      auto range = RowRange::fromFirstAndLastRow(6, 7);
      auto insertPoint = list.end();

      auto it = findFirstElementToMerge(list.begin(), insertPoint, list.end(), range);

      REQUIRE( it == list.end() );
    }
  }

  SECTION("given list {[0,0],[3,3]}")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 0) );
    list.emplace_back( RowRange::fromFirstAndLastRow(3, 3) );

    SECTION("for [2,2] insert point is second, result is second")
    {
      auto range = RowRange::fromFirstAndLastRow(2, 2);
      auto insertPoint = list.begin() + 1;

      auto it = findFirstElementToMerge(list.begin(), insertPoint, list.end(), range);

      REQUIRE( it == (list.begin() + 1) );
    }
  }
}

TEST_CASE("findLastElementToMerge")
{
  RowRangeListContainer list;

  SECTION("given single element list {[0,1]}")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );

    SECTION("last element with [1,2] is first element")
    {
      auto range = RowRange::fromFirstAndLastRow(1, 2);

      auto it = findLastElementToMerge(list.begin(), list.end(), range);

      REQUIRE( it == list.cbegin() );
    }
  }

  SECTION("given list {[0,1],[6,8]}")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );
    list.emplace_back( RowRange::fromFirstAndLastRow(6, 8) );

    SECTION("last element with [1,2] is first element")
    {
      auto range = RowRange::fromFirstAndLastRow(1, 2);

      auto it = findLastElementToMerge(list.begin(), list.end(), range);

      REQUIRE( it == list.begin() );
    }

    SECTION("last element with [1,6] is second element")
    {
      auto range = RowRange::fromFirstAndLastRow(1, 6);

      auto it = findLastElementToMerge(list.begin(), list.end(), range);

      REQUIRE( it == (list.begin() + 1) );
    }
  }
}

TEST_CASE("mergeRangesUntil")
{
  RowRangeListContainer list;

  SECTION("given single element list {[0,1]}")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );

    SECTION("[1,2] merges: {[0,2]}")
    {
      auto range = RowRange::fromFirstAndLastRow(1, 2);

      auto it = mergeRangesUntil(list.begin(), list.end(), range);

      REQUIRE( it == list.end() );
      REQUIRE( list.size() == 1 );
      REQUIRE( list[0].firstRow() == 0 );
      REQUIRE( list[0].lastRow() == 2 );
    }
  }

  SECTION("given list {[0,1],[6,8]}")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );
    list.emplace_back( RowRange::fromFirstAndLastRow(6, 8) );

    SECTION("[2,4] merges: {[0,4],[6,8]}")
    {
      auto range = RowRange::fromFirstAndLastRow(2, 4);

      auto it = mergeRangesUntil(list.begin(), list.end(), range);

      REQUIRE( it == list.end() );
      REQUIRE( list.size() == 2 );
      REQUIRE( list[0].firstRow() == 0 );
      REQUIRE( list[0].lastRow() == 4 );
      REQUIRE( list[1].firstRow() == 6 );
      REQUIRE( list[1].lastRow() == 8 );
    }
  }

  SECTION("given list {[0,1],[3,4],[6,7],[9,10]}")
  {
    list.emplace_back( RowRange::fromFirstAndLastRow(0, 1) );
    list.emplace_back( RowRange::fromFirstAndLastRow(3, 4) );
    list.emplace_back( RowRange::fromFirstAndLastRow(6, 7) );
    list.emplace_back( RowRange::fromFirstAndLastRow(9, 10) );

    SECTION("[0,4] merges: {[0,4],[6,7],[9,10],X}")
    {
      auto range = RowRange::fromFirstAndLastRow(0, 4);

      auto it = mergeRangesUntil(list.begin(), list.end(), range);

      REQUIRE( it == (list.begin() + 3) );
      REQUIRE( list.size() == 4 );
      REQUIRE( list[0].firstRow() == 0 );
      REQUIRE( list[0].lastRow() == 4 );
      REQUIRE( list[1].firstRow() == 6 );
      REQUIRE( list[1].lastRow() == 7 );
      REQUIRE( list[2].firstRow() == 9 );
      REQUIRE( list[2].lastRow() == 10 );
    }

    SECTION("[0,10] merges: {[0,10]}")
    {
      auto range = RowRange::fromFirstAndLastRow(0, 10);

      auto it = mergeRangesUntil(list.begin(), list.end(), range);

      REQUIRE( it == (list.begin() + 1) );
      REQUIRE( list.size() == 4 );
      REQUIRE( list[0].firstRow() == 0 );
      REQUIRE( list[0].lastRow() == 10 );
    }
  }
}
