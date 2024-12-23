// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "ReadOnlyTableModel.h"
#include "RemoveRowsTableModel.h"
#include "Mdt/ItemModel/Helpers.h"
#include <QItemSelectionModel>
#include <QStringListModel>
#include <QStringList>
#include <QLatin1String>

using namespace Mdt::ItemModel;

void populateModel(RemoveRowsTableModel & model, const RemoveRowsTableModel::Table & tableData)
{
  model.setTable(tableData);
}

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

void selectItem(QItemSelectionModel & selectionModel, int row, int column)
{
  const auto *model = selectionModel.model();
  assert( model != nullptr );

  assert( row >= 0 );
  assert( row < model->rowCount() );
  assert( column >= 0 );
  assert( column < model->columnCount() );

  QModelIndex index = model->index(row, column);

  selectionModel.select(index, QItemSelectionModel::Select);
}


TEST_CASE("modelRowAndColumnAreInRange")
{
  ReadOnlyTableModel model;
  model.setTable({{1,"A"}});

  SECTION("-1,-1 is not in range")
  {
    REQUIRE( !modelRowAndColumnAreInRange(model, -1, -1) );
  }

  SECTION("-1,0 is not in range")
  {
    REQUIRE( !modelRowAndColumnAreInRange(model, -1, 0) );
  }

  SECTION("0,-1 is not in range")
  {
    REQUIRE( !modelRowAndColumnAreInRange(model, 0, -1) );
  }

  SECTION("0,0 is in range")
  {
    REQUIRE( modelRowAndColumnAreInRange(model, 0, 0) );
  }

  SECTION("0,1 is in range")
  {
    REQUIRE( modelRowAndColumnAreInRange(model, 0, 1) );
  }

  SECTION("1,0 is not in range")
  {
    REQUIRE( !modelRowAndColumnAreInRange(model, 1, 0) );
  }

  SECTION("0,2 is not in range")
  {
    REQUIRE( !modelRowAndColumnAreInRange(model, 0, 2) );
  }
}

TEST_CASE("getModelData")
{
  QStringListModel model(QStringList{QLatin1String("A"),QLatin1String("B"),QLatin1String("C")});

  REQUIRE( getModelData(model, 0, 0) == QLatin1String("A") );
  REQUIRE( getModelData(model, 1, 0) == QLatin1String("B") );
  REQUIRE( getModelData(model, 2, 0) == QLatin1String("C") );
}

TEST_CASE("setModelData")
{
  QStringListModel model(QStringList{QLatin1String("A"),QLatin1String("B"),QLatin1String("C")});

  REQUIRE( getModelData(model, 0, 0) == QLatin1String("A") );
  REQUIRE( getModelData(model, 1, 0) == QLatin1String("B") );
  REQUIRE( getModelData(model, 2, 0) == QLatin1String("C") );

  REQUIRE( setModelData( model, 1, 0, QString::fromLatin1("Z") ) );

  REQUIRE( getModelData(model, 0, 0) == QLatin1String("A") );
  REQUIRE( getModelData(model, 1, 0) == QLatin1String("Z") );
  REQUIRE( getModelData(model, 2, 0) == QLatin1String("C") );
}

TEST_CASE("prependRowToModel")
{
  QStringListModel model;
  REQUIRE( model.rowCount() == 0 );

  /*
   * Prepend a row
   * ---
   * | |
   * ---
   */
  REQUIRE( prependRowToModel(model) );
  REQUIRE( model.rowCount() == 1 );

  /*
   * Set data to this new row
   * ---
   * |A|
   * ---
   */
  REQUIRE( setModelData( model, 0, 0, QString::fromLatin1("A") ) );
  REQUIRE( getModelData(model, 0, 0) == QLatin1String("A") );

  /*
   * Prepend a row
   * ---
   * | |
   * ---
   * |A|
   * ---
   */
  REQUIRE( prependRowToModel(model) );
  REQUIRE( model.rowCount() == 2 );
  REQUIRE( getModelData(model, 0, 0).toString().isEmpty() );
  REQUIRE( getModelData(model, 1, 0) == QLatin1String("A") );
}

TEST_CASE("appendRowToModel")
{
  QStringListModel model;
  REQUIRE( model.rowCount() == 0 );

  /*
   * Append a row
   * ---
   * | |
   * ---
   */
  REQUIRE( appendRowToModel(model) );
  REQUIRE( model.rowCount() == 1 );

  /*
   * Set data to this first row
   * ---
   * |A|
   * ---
   */
  REQUIRE( setModelData( model, 0, 0, QString::fromLatin1("A") ) );
  REQUIRE( getModelData(model, 0, 0) == QLatin1String("A") );

  /*
   * Append a row
   * ---
   * |A|
   * ---
   * | |
   * ---
   */
  REQUIRE( appendRowToModel(model) );
  REQUIRE( model.rowCount() == 2 );
  REQUIRE( getModelData(model, 0, 0) == QLatin1String("A") );
  REQUIRE( getModelData(model, 1, 0).toString().isEmpty() );
}

TEST_CASE("removeFirstRowFromModel")
{
  QStringListModel model;

  SECTION("removing from an empty model returns false")
  {
    REQUIRE( !removeFirstRowFromModel(model) );
  }

  SECTION("remove first row")
  {
    model.setStringList( {QLatin1String("A"),QLatin1String("B")} );
    REQUIRE( model.rowCount() == 2 );

    REQUIRE( removeFirstRowFromModel(model) );

    REQUIRE( model.rowCount() == 1 );
    REQUIRE( getModelData(model, 0, 0) == QLatin1String("B") );
  }
}

TEST_CASE("removeLastRowFromModel")
{
  QStringListModel model;

  SECTION("removing from an empty model returns false")
  {
    REQUIRE( !removeLastRowFromModel(model) );
  }

  SECTION("remove last row")
  {
    model.setStringList( {QLatin1String("A"),QLatin1String("B")} );
    REQUIRE( model.rowCount() == 2 );

    REQUIRE( removeLastRowFromModel(model) );

    REQUIRE( model.rowCount() == 1 );
    REQUIRE( getModelData(model, 0, 0) == QLatin1String("A") );
  }
}

/*
 * Some tests are also done in ItemViewHelpersTest
 *
 * As example, selecting all items is done there
 */
TEST_CASE("removeSelectedRows")
{
  RemoveRowsTableModel model;
  populateModel(model, {{1,"A"},{2,"B"},{3,"C"},{4,"D"}});

  QItemSelectionModel selectionModel(&model);

  SECTION("select 0,0 - REM row 0")
  {
    selectItem(selectionModel, 0, 0);

    REQUIRE( removeSelectedRows(&selectionModel) );

    REQUIRE( model.rowCount() == 3 );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("B") );
    REQUIRE( getModelData(model, 1, 1) == QLatin1String("C") );
    REQUIRE( getModelData(model, 2, 1) == QLatin1String("D") );
  }

  SECTION("select 0,0|0,1 - REM row 0")
  {
    selectItem(selectionModel, 0, 0);
    selectItem(selectionModel, 0, 1);

    REQUIRE( removeSelectedRows(&selectionModel) );

    REQUIRE( model.rowCount() == 3 );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("B") );
    REQUIRE( getModelData(model, 1, 1) == QLatin1String("C") );
    REQUIRE( getModelData(model, 2, 1) == QLatin1String("D") );
  }

  SECTION("select 0,0|1,0 - REM rows 1 to 0")
  {
    selectItem(selectionModel, 0, 0);
    selectItem(selectionModel, 1, 0);

    REQUIRE( removeSelectedRows(&selectionModel) );

    REQUIRE( model.rowCount() == 2 );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("C") );
    REQUIRE( getModelData(model, 1, 1) == QLatin1String("D") );
  }

  SECTION("select 0,0|2,0|3,0 - REM rows [3 to 2][0]")
  {
    selectItem(selectionModel, 0, 0);
    selectItem(selectionModel, 2, 0);
    selectItem(selectionModel, 3, 0);

    REQUIRE( removeSelectedRows(&selectionModel) );

    REQUIRE( model.rowCount() == 1 );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("B") );
  }

  SECTION("select 3,0|0,0|2,0 - REM rows [3 to 2][0]")
  {
    selectItem(selectionModel, 3, 0);
    selectItem(selectionModel, 0, 0);
    selectItem(selectionModel, 2, 0);

    REQUIRE( removeSelectedRows(&selectionModel) );

    REQUIRE( model.rowCount() == 1 );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("B") );
  }
}

TEST_CASE("itemSelectionIsSingleRow")
{
  QItemSelection selection;
  ReadOnlyTableModel model;
  model.setTable({
    {1,"A"},
    {2,"B"},
    {3,"C"}
  });

  SECTION("an empty selection is not single row")
  {
    REQUIRE( !itemSelectionIsSingleRow(selection) );
  }

  SECTION("1 item is a single row")
  {
    addItemRangeToSelection(model, {0,0}, {0,0}, selection);

    REQUIRE( itemSelectionIsSingleRow(selection) );
  }

  SECTION("1 range with 2 columns that is a single row")
  {
    addItemRangeToSelection(model, {0,0}, {0,1}, selection);

    REQUIRE( itemSelectionIsSingleRow(selection) );
  }

  SECTION("1 range that is not a single row")
  {
    addItemRangeToSelection(model, {0,0}, {1,0}, selection);

    REQUIRE( !itemSelectionIsSingleRow(selection) );
  }

  SECTION("2 ranges representing the same row")
  {
    addItemRangeToSelection(model, {0,0}, {0,0}, selection);
    addItemRangeToSelection(model, {0,0}, {0,1}, selection);

    REQUIRE( itemSelectionIsSingleRow(selection) );
  }

  SECTION("2 ranges - second is not a single row")
  {
    addItemRangeToSelection(model, {0,0}, {0,0}, selection);
    addItemRangeToSelection(model, {0,0}, {1,0}, selection);

    REQUIRE( !itemSelectionIsSingleRow(selection) );
  }

  SECTION("3 ranges representing the same row")
  {
    addItemRangeToSelection(model, {0,0}, {0,0}, selection);
    addItemRangeToSelection(model, {0,0}, {0,1}, selection);
    addItemRangeToSelection(model, {0,0}, {0,0}, selection);

    REQUIRE( itemSelectionIsSingleRow(selection) );
  }

  SECTION("2 single row ranges - second has different row")
  {
    addItemRangeToSelection(model, {0,0}, {0,0}, selection);
    addItemRangeToSelection(model, {1,0}, {1,0}, selection);

    REQUIRE( !itemSelectionIsSingleRow(selection) );
  }

  SECTION("3 single row ranges - third has different row")
  {
    addItemRangeToSelection(model, {0,0}, {0,0}, selection);
    addItemRangeToSelection(model, {0,0}, {0,0}, selection);
    addItemRangeToSelection(model, {1,0}, {1,0}, selection);

    REQUIRE( !itemSelectionIsSingleRow(selection) );
  }

  SECTION("4 single row ranges - fourth has different row")
  {
    addItemRangeToSelection(model, {0,0}, {0,0}, selection);
    addItemRangeToSelection(model, {0,0}, {0,0}, selection);
    addItemRangeToSelection(model, {0,0}, {0,0}, selection);
    addItemRangeToSelection(model, {1,0}, {1,0}, selection);

    REQUIRE( !itemSelectionIsSingleRow(selection) );
  }
}

TEST_CASE("updateRowInSingleRowItemSelection")
{
  QItemSelection selection;
  ReadOnlyTableModel model;
  model.setTable({
    {1,"A"},
    {2,"B"},
    {3,"C"}
  });

  SECTION("update single index 0,0 to 1,0")
  {
    addItemRangeToSelection(model, {0,0}, {0,0}, selection);

    updateRowInSingleRowItemSelection(1, selection);

    REQUIRE( selection.count() == 1 );
    REQUIRE( selection.at(0).top() == 1 );
    REQUIRE( selection.at(0).bottom() == 1 );
    REQUIRE( selection.at(0).left() == 0 );
    REQUIRE( selection.at(0).right() == 0 );
  }

  /*
   * |0||S|S|  ->  |1||S|S|
   */
  SECTION("update a single range 0,0-0,1 to 1,0-1,1")
  {
    addItemRangeToSelection(model, {0,0}, {0,1}, selection);

    updateRowInSingleRowItemSelection(1, selection);

    REQUIRE( selection.count() == 1 );
    REQUIRE( selection.at(0).top() == 1 );
    REQUIRE( selection.at(0).bottom() == 1 );
    REQUIRE( selection.at(0).left() == 0 );
    REQUIRE( selection.at(0).right() == 1 );
  }

  /*
   * |0||S| |      |1||S| |
   * |0|| |S|  ->  |1|| |S|
   */
  SECTION("update a 2 range selection")
  {
    addItemRangeToSelection(model, {0,0}, {0,0}, selection);
    addItemRangeToSelection(model, {0,1}, {0,1}, selection);

    updateRowInSingleRowItemSelection(1, selection);

    REQUIRE( selection.count() == 2 );
    REQUIRE( selection.at(0).top() == 1 );
    REQUIRE( selection.at(0).bottom() == 1 );
    REQUIRE( selection.at(0).left() == 0 );
    REQUIRE( selection.at(0).right() == 0 );
    REQUIRE( selection.at(1).top() == 1 );
    REQUIRE( selection.at(1).bottom() == 1 );
    REQUIRE( selection.at(1).left() == 1 );
    REQUIRE( selection.at(1).right() == 1 );
  }
}
