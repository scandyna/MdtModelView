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
#include "ReadOnlyTableModel.h"
#include "Mdt/ItemModel/RowSelectionHelpers.h"
#include <QItemSelectionRange>
#include <cassert>

using namespace Mdt::ItemModel;

void populateModel(ReadOnlyTableModel & model, const ReadOnlyTableModel::Table & tableData)
{
  model.setTable(tableData);
}

struct RowColumn
{
  int row;
  int column;
};

QItemSelectionRange makeItemSelectionRange(const QAbstractTableModel & model, const RowColumn & topLeft, const RowColumn & bottomRight)
{
  assert( topLeft.row >= 0 );
  assert( topLeft.row < model.rowCount() );
  assert( topLeft.column >= 0 );
  assert( topLeft.column < model.columnCount() );
  assert( bottomRight.row >= 0 );
  assert( bottomRight.row < model.rowCount() );
  assert( bottomRight.column >= 0 );
  assert( bottomRight.column < model.columnCount() );

  const QModelIndex topLeftIndex = model.index(topLeft.row, topLeft.column);
  const QModelIndex bottomRightIndex = model.index(bottomRight.row, bottomRight.column);

  return QItemSelectionRange(topLeftIndex, bottomRightIndex);
}


TEST_CASE("rowRangeFromItemSelectionRange")
{
  ReadOnlyTableModel model;
  QItemSelectionRange itemRange;

  populateModel(model,
  {
    {1,"A"},
    {2,"B"},
    {3,"C"}
  });

  SECTION("1 item range")
  {
    itemRange = makeItemSelectionRange(model, {0,0}, {0,0});

    RowRange rowRange = rowRangeFromItemSelectionRange(itemRange);

    REQUIRE( rowRange.firstRow() == 0 );
    REQUIRE( rowRange.lastRow() == 0 );
  }

  SECTION("1 row 2 columns item range")
  {
    itemRange = makeItemSelectionRange(model, {0,0}, {0,1});

    RowRange rowRange = rowRangeFromItemSelectionRange(itemRange);

    REQUIRE( rowRange.firstRow() == 0 );
    REQUIRE( rowRange.lastRow() == 0 );
  }

  SECTION("all items")
  {
    itemRange = makeItemSelectionRange(model, {0,0}, {2,1});

    RowRange rowRange = rowRangeFromItemSelectionRange(itemRange);

    REQUIRE( rowRange.firstRow() == 0 );
    REQUIRE( rowRange.lastRow() == 2 );
  }
}
