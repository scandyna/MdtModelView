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
#include "Mdt/ItemModel/RowListView.h"
#include <QStringList>
#include <QStringListModel>
#include <QItemSelection>
#include <QLatin1String>
#include <vector>

using namespace Mdt::ItemModel;


TEST_CASE("isEmpty")
{
  RowRangeList rangeList;

  SECTION("empty")
  {
    RowListView view(rangeList);

    REQUIRE( view.isEmpty() );
  }

  SECTION("1 range with 1 row")
  {
    rangeList.addRange( RowRange::fromFirstAndLastRow(0,0) );
    RowListView view(rangeList);

    REQUIRE( !view.isEmpty() );
  }
}

TEST_CASE("isSingleRow")
{
  RowRangeList rangeList;

  SECTION("empty")
  {
    RowListView view(rangeList);

    REQUIRE( !view.isSingleRow() );
  }

  SECTION("1 range with 1 row")
  {
    rangeList.addRange( RowRange::fromFirstAndLastRow(0,0) );
    RowListView view(rangeList);

    REQUIRE( view.isSingleRow() );
  }

  SECTION("1 range with 2 rows")
  {
    rangeList.addRange( RowRange::fromFirstAndLastRow(0,1) );
    RowListView view(rangeList);

    REQUIRE( !view.isSingleRow() );
  }

  SECTION("2 ranges with 1 row each")
  {
    rangeList.addRange( RowRange::fromFirstAndLastRow(0,0) );
    rangeList.addRange( RowRange::fromFirstAndLastRow(1,1) );
    RowListView view(rangeList);

    REQUIRE( !view.isSingleRow() );
  }
}

TEST_CASE("iterateOverRangeList")
{
  RowRangeList rangeList;
  rangeList.addRange( RowRange::fromFirstAndLastRow(0,2) );
  rangeList.addRange( RowRange::fromFirstAndLastRow(4,5) );

  RowListView view(rangeList);
  std::vector<int> rowList;

  for(int row : view){
    rowList.push_back(row);
  }

  REQUIRE( rowList.size() == 5 );
  REQUIRE( rowList[0] == 0 );
  REQUIRE( rowList[1] == 1 );
  REQUIRE( rowList[2] == 2 );
  REQUIRE( rowList[3] == 4 );
  REQUIRE( rowList[4] == 5 );
}

TEST_CASE("iterateOverRowSelection")
{
  QStringListModel model({QLatin1String("A"),QLatin1String("B"),QLatin1String("C")});
  QItemSelection itemSelection( model.index(1,0), model.index(2,0) );
  auto rowSelection = RowSelection::fromItemSelection(itemSelection);

  RowListView view(rowSelection);
  std::vector<int> rowList;

  for(int row : view){
    rowList.push_back(row);
  }

  REQUIRE( rowList.size() == 2 );
  REQUIRE( rowList[0] == 1 );
  REQUIRE( rowList[1] == 2 );
}
