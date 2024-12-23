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
#include "Mdt/ItemModel/RowRangeList.h"

using namespace Mdt::ItemModel;


TEST_CASE("emptyList")
{
  RowRangeList list;

  REQUIRE( list.rangeCount() == 0 );
  REQUIRE( list.isEmpty() );
}

TEST_CASE("addRange")
{
  RowRangeList list;

  SECTION("add a range to an empty list")
  {
    list.addRange( RowRange::fromFirstAndLastRow(0,1) );

    REQUIRE( list.rangeCount() == 1 );
    REQUIRE( list.rangeAt(0).firstRow() == 0 );
    REQUIRE( list.rangeAt(0).lastRow() == 1 );
  }

  SECTION("add 2 disjoint ranges")
  {
    list.addRange( RowRange::fromFirstAndLastRow(0,1) );
    list.addRange( RowRange::fromFirstAndLastRow(3,4) );

    REQUIRE( list.rangeCount() == 2 );
    REQUIRE( list.rangeAt(0).firstRow() == 0 );
    REQUIRE( list.rangeAt(0).lastRow() == 1 );
    REQUIRE( list.rangeAt(1).firstRow() == 3 );
    REQUIRE( list.rangeAt(1).lastRow() == 4 );
  }

  SECTION("add 2 disjoint ranges in reverse order")
  {
    list.addRange( RowRange::fromFirstAndLastRow(3,4) );
    list.addRange( RowRange::fromFirstAndLastRow(0,1) );

    REQUIRE( list.rangeCount() == 2 );
    REQUIRE( list.rangeAt(0).firstRow() == 0 );
    REQUIRE( list.rangeAt(0).lastRow() == 1 );
    REQUIRE( list.rangeAt(1).firstRow() == 3 );
    REQUIRE( list.rangeAt(1).lastRow() == 4 );
  }

  SECTION("add 2 ranges [0,1] [2,3] resulting in {[0,3]}")
  {
    list.addRange( RowRange::fromFirstAndLastRow(0,1) );
    list.addRange( RowRange::fromFirstAndLastRow(2,3) );

    REQUIRE( list.rangeCount() == 1 );
    REQUIRE( list.rangeAt(0).firstRow() == 0 );
    REQUIRE( list.rangeAt(0).lastRow() == 3 );
  }

  SECTION("starting with {[3,4]}")
  {
    list.addRange( RowRange::fromFirstAndLastRow(3,4) );

    REQUIRE( list.rangeCount() == 1 );
    REQUIRE( list.rangeAt(0).firstRow() == 3 );
    REQUIRE( list.rangeAt(0).lastRow() == 4 );

    SECTION("adding [0,1] results in {[0,1],[3,4]}")
    {
      list.addRange( RowRange::fromFirstAndLastRow(0,1) );

      REQUIRE( list.rangeCount() == 2 );
      REQUIRE( list.rangeAt(0).firstRow() == 0 );
      REQUIRE( list.rangeAt(0).lastRow() == 1 );
      REQUIRE( list.rangeAt(1).firstRow() == 3 );
      REQUIRE( list.rangeAt(1).lastRow() == 4 );
    }

    SECTION("adding [1,2] results in {[1,4]}")
    {
      list.addRange( RowRange::fromFirstAndLastRow(1,2) );

      REQUIRE( list.rangeCount() == 1 );
      REQUIRE( list.rangeAt(0).firstRow() == 1 );
      REQUIRE( list.rangeAt(0).lastRow() == 4 );
    }

    SECTION("adding [3,4] results in {[3,4]}")
    {
      list.addRange( RowRange::fromFirstAndLastRow(3,4) );

      REQUIRE( list.rangeCount() == 1 );
      REQUIRE( list.rangeAt(0).firstRow() == 3 );
      REQUIRE( list.rangeAt(0).lastRow() == 4 );
    }

    SECTION("adding [3,5] results in {[3,5]}")
    {
      list.addRange( RowRange::fromFirstAndLastRow(3,5) );

      REQUIRE( list.rangeCount() == 1 );
      REQUIRE( list.rangeAt(0).firstRow() == 3 );
      REQUIRE( list.rangeAt(0).lastRow() == 5 );
    }

    SECTION("adding [6,7] results in {[3,4],[6,7]}")
    {
      list.addRange( RowRange::fromFirstAndLastRow(6,7) );

      REQUIRE( list.rangeCount() == 2 );
      REQUIRE( list.rangeAt(0).firstRow() == 3 );
      REQUIRE( list.rangeAt(0).lastRow() == 4 );
      REQUIRE( list.rangeAt(1).firstRow() == 6 );
      REQUIRE( list.rangeAt(1).lastRow() == 7 );
    }

    SECTION("adding [0,5] results in {[0,5]}")
    {
      list.addRange( RowRange::fromFirstAndLastRow(0,5) );

      REQUIRE( list.rangeCount() == 1 );
      REQUIRE( list.rangeAt(0).firstRow() == 0 );
      REQUIRE( list.rangeAt(0).lastRow() == 5 );
    }
  }

  SECTION("starting with {[0,1],[6,8]}")
  {
    list.addRange( RowRange::fromFirstAndLastRow(0,1) );
    list.addRange( RowRange::fromFirstAndLastRow(6,8) );

    REQUIRE( list.rangeCount() == 2 );
    REQUIRE( list.rangeAt(0).firstRow() == 0 );
    REQUIRE( list.rangeAt(0).lastRow() == 1 );
    REQUIRE( list.rangeAt(1).firstRow() == 6 );
    REQUIRE( list.rangeAt(1).lastRow() == 8 );

    SECTION("adding [3,4] will result in {[0,1],[3,4],[6,8]}")
    {
      list.addRange( RowRange::fromFirstAndLastRow(3,4) );

      REQUIRE( list.rangeCount() == 3 );
      REQUIRE( list.rangeAt(0).firstRow() == 0 );
      REQUIRE( list.rangeAt(0).lastRow() == 1 );
      REQUIRE( list.rangeAt(1).firstRow() == 3 );
      REQUIRE( list.rangeAt(1).lastRow() == 4 );
      REQUIRE( list.rangeAt(2).firstRow() == 6 );
      REQUIRE( list.rangeAt(2).lastRow() == 8 );
    }

    SECTION("adding [2,4] will result in {[0,4],[6,8]}")
    {
      list.addRange( RowRange::fromFirstAndLastRow(2,4) );

      REQUIRE( list.rangeCount() == 2 );
      REQUIRE( list.rangeAt(0).firstRow() == 0 );
      REQUIRE( list.rangeAt(0).lastRow() == 4 );
      REQUIRE( list.rangeAt(1).firstRow() == 6 );
      REQUIRE( list.rangeAt(1).lastRow() == 8 );
    }

    SECTION("adding [0,4] will result in {[0,4],[6,8]}")
    {
      list.addRange( RowRange::fromFirstAndLastRow(0,4) );

      REQUIRE( list.rangeCount() == 2 );
      REQUIRE( list.rangeAt(0).firstRow() == 0 );
      REQUIRE( list.rangeAt(0).lastRow() == 4 );
      REQUIRE( list.rangeAt(1).firstRow() == 6 );
      REQUIRE( list.rangeAt(1).lastRow() == 8 );
    }
  }

  SECTION("add single row ranges [3,3] [0,0] [2,2] results in {[0,0],[2,3]}")
  {
    list.addRange( RowRange::fromFirstAndLastRow(3,3) );
    list.addRange( RowRange::fromFirstAndLastRow(0,0) );
    list.addRange( RowRange::fromFirstAndLastRow(2,2) );

    REQUIRE( list.rangeCount() == 2 );
    REQUIRE( list.rangeAt(0).firstRow() == 0 );
    REQUIRE( list.rangeAt(0).lastRow() == 0 );
    REQUIRE( list.rangeAt(1).firstRow() == 2 );
    REQUIRE( list.rangeAt(1).lastRow() == 3 );
  }
}
