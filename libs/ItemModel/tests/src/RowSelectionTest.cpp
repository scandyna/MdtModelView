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
#include "Mdt/ItemModel/RowSelection.h"
#include <QItemSelection>
#include <QItemSelectionRange>
#include <cassert>

// #include <QDebug>

using namespace Mdt::ItemModel;

void populateModel(ReadOnlyTableModel & model, const ReadOnlyTableModel::Table & tableData)
{
  model.setTable(tableData);
}

// void addItemToSelection(const QAbstractTableModel & model, int row, int column, QItemSelection & selection)
// {
//   assert( row >= 0 );
//   assert( row < model.rowCount() );
//   assert( column >= 0 );
//   assert( column < model.columnCount() );
// 
//   const QModelIndex topLeft = model.index(row, column);
//   const QModelIndex bottomRight = topLeft;
// 
//   selection.select(topLeft, bottomRight);
// }

struct RowColumn
{
  int row;
  int column;
};

void addItemRangeToSelection(const QAbstractTableModel & model, const RowColumn & topLeft, const RowColumn & bottomRight, QItemSelection & selection)
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

  QItemSelectionRange range(topLeftIndex, bottomRightIndex);

  selection.append(range);
}


TEST_CASE("fromItemSelection")
{
  ReadOnlyTableModel model;
  QItemSelection itemSelection;

  /*
   * --------
   * |0||1|A|
   * --------
   * |1||2|B|
   * --------
   * |2||3|C|
   * --------
   * |3||4|D|
   * --------
   * |4||5|E|
   * --------
   * |5||6|F|
   * --------
   */
  populateModel(model,
  {
    {1,"A"},
    {2,"B"},
    {3,"C"},
    {4,"D"},
    {5,"E"},
    {6,"F"}
  });

  SECTION("empty selection")
  {
    auto rowSelection = RowSelection::fromItemSelection(itemSelection);

    REQUIRE( rowSelection.isEmpty() );
  }

  /*
   * --------
   * |0||S|S|
   * --------
   *
   * [0]
   */
  SECTION("Select 2 items in same row results in 1 row")
  {
    addItemRangeToSelection(model, {0,0}, {0,0}, itemSelection);
    addItemRangeToSelection(model, {0,1}, {0,1}, itemSelection);
    REQUIRE( itemSelection.count() == 2 );

    auto rowSelection = RowSelection::fromItemSelection(itemSelection);

    REQUIRE( rowSelection.rangeCount() == 1 );
    REQUIRE( rowSelection.rangeAt(0).firstRow() == 0 );
    REQUIRE( rowSelection.rangeAt(0).lastRow() == 0 );
  }

  /*
   * --------
   * |0||S| |
   * --------
   * |1|| |S|
   * --------
   * |2||S| |
   * --------
   *
   * [0,2]
   */
  SECTION("select items in contiguous rows results in 1 range of rows")
  {
    addItemRangeToSelection(model, {0,0}, {0,0}, itemSelection);
    addItemRangeToSelection(model, {1,1}, {1,1}, itemSelection);
    addItemRangeToSelection(model, {2,0}, {2,0}, itemSelection);
    REQUIRE( itemSelection.count() == 3 );

    auto rowSelection = RowSelection::fromItemSelection(itemSelection);

    REQUIRE( rowSelection.rangeCount() == 1 );
    REQUIRE( rowSelection.rangeAt(0).firstRow() == 0 );
    REQUIRE( rowSelection.rangeAt(0).lastRow() == 2 );
  }

  /*
   * --------
   * |0||S| |
   * --------
   * |1|| |S|
   * --------
   * |2|| | |
   * --------
   * |3||S| |
   * --------
   * |4||S|S|
   * --------
   * |5||S| |
   * --------
   *
   * {[0,1],[3,5]}
   */
  SECTION("select items in 2 discontiguous rows results in 2 ranges of rows")
  {
    addItemRangeToSelection(model, {0,0}, {0,0}, itemSelection);
    addItemRangeToSelection(model, {1,1}, {1,1}, itemSelection);
    addItemRangeToSelection(model, {3,0}, {3,0}, itemSelection);
    addItemRangeToSelection(model, {4,0}, {4,1}, itemSelection);
    addItemRangeToSelection(model, {5,0}, {5,0}, itemSelection);
    REQUIRE( itemSelection.count() == 5 );

    auto rowSelection = RowSelection::fromItemSelection(itemSelection);

    REQUIRE( rowSelection.rangeCount() == 2 );
    REQUIRE( rowSelection.rangeAt(0).firstRow() == 0 );
    REQUIRE( rowSelection.rangeAt(0).lastRow() == 1 );
    REQUIRE( rowSelection.rangeAt(1).firstRow() == 3 );
    REQUIRE( rowSelection.rangeAt(1).lastRow() == 5 );
  }

  /*
   * --------
   * |0||S|S|
   * --------
   * |1||S|S|
   * --------
   * |2||S|S|
   * --------
   *
   * [0,2]
   */
  SECTION("select all (one item selection range)")
  {
    addItemRangeToSelection(model, {0,0}, {2,1}, itemSelection);
    REQUIRE( itemSelection.count() == 1 );

    auto rowSelection = RowSelection::fromItemSelection(itemSelection);

    REQUIRE( rowSelection.rangeCount() == 1 );
    REQUIRE( rowSelection.rangeAt(0).firstRow() == 0 );
    REQUIRE( rowSelection.rangeAt(0).lastRow() == 2 );
  }
}
