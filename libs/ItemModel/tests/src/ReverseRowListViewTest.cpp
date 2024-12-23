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
#include "Mdt/ItemModel/ReverseRowListView.h"
#include <QStringList>
#include <QStringListModel>
#include <QItemSelection>
#include <QLatin1String>
#include <vector>

using namespace Mdt::ItemModel;


TEST_CASE("iterateOverRangeList")
{
  RowRangeList rangeList;
  rangeList.addRange( RowRange::fromFirstAndLastRow(0,2) );
  rangeList.addRange( RowRange::fromFirstAndLastRow(4,5) );

  ReverseRowListView view(rangeList);
  std::vector<int> rowList;

  for(int row : view){
    rowList.push_back(row);
  }

  REQUIRE( rowList.size() == 5 );
  REQUIRE( rowList[0] == 5 );
  REQUIRE( rowList[1] == 4 );
  REQUIRE( rowList[2] == 2 );
  REQUIRE( rowList[3] == 1 );
  REQUIRE( rowList[4] == 0 );
}

TEST_CASE("iterateOverRowSelection")
{
  QStringListModel model({QLatin1String("A"),QLatin1String("B"),QLatin1String("C")});
  QItemSelection itemSelection( model.index(1,0), model.index(2,0) );
  auto rowSelection = RowSelection::fromItemSelection(itemSelection);

  ReverseRowListView view(rowSelection);
  std::vector<int> rowList;

  for(int row : view){
    rowList.push_back(row);
  }

  REQUIRE( rowList.size() == 2 );
  REQUIRE( rowList[0] == 2 );
  REQUIRE( rowList[1] == 1 );
}
