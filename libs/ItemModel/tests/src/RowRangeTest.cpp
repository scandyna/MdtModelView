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
#include "Mdt/ItemModel/RowRange.h"

using namespace Mdt::ItemModel;


TEST_CASE("firstAndLastRowIsValidRange")
{
  SECTION("[0,0] is valid")
  {
    REQUIRE( RowRange::firstAndLastRowIsValidRange(0, 0) );
  }

  SECTION("[0,1] is valid")
  {
    REQUIRE( RowRange::firstAndLastRowIsValidRange(0, 1) );
  }

  SECTION("[0,5] is valid")
  {
    REQUIRE( RowRange::firstAndLastRowIsValidRange(0, 5) );
  }

  SECTION("first < 0 is not valid")
  {
    REQUIRE( !RowRange::firstAndLastRowIsValidRange(-1, 0) );
  }

  SECTION("last < 0 is not valid")
  {
    REQUIRE( !RowRange::firstAndLastRowIsValidRange(0, -1) );
  }

  SECTION("first > last is not valid")
  {
    REQUIRE( !RowRange::firstAndLastRowIsValidRange(1, 0) );
  }
}

TEST_CASE("fromFirstAndLastRow")
{
  SECTION("[0,0]")
  {
    auto range = RowRange::fromFirstAndLastRow(0, 0);

    REQUIRE( range.firstRow() == 0 );
    REQUIRE( range.lastRow() == 0 );
  }

  SECTION("[0,1]")
  {
    auto range = RowRange::fromFirstAndLastRow(0, 1);

    REQUIRE( range.firstRow() == 0 );
    REQUIRE( range.lastRow() == 1 );
  }
}

TEST_CASE("rowCount")
{
  SECTION("[0,0] counts 1 row")
  {
    auto range = RowRange::fromFirstAndLastRow(0, 0);

    REQUIRE( range.rowCount() == 1 );
  }

  SECTION("[0,1] counts 2 rows")
  {
    auto range = RowRange::fromFirstAndLastRow(0, 1);

    REQUIRE( range.rowCount() == 2 );
  }

  SECTION("[1,1] counts 1 row")
  {
    auto range = RowRange::fromFirstAndLastRow(1, 1);

    REQUIRE( range.rowCount() == 1 );
  }

  SECTION("[1,2] counts 2 rows")
  {
    auto range = RowRange::fromFirstAndLastRow(1, 2);

    REQUIRE( range.rowCount() == 2 );
  }
}
