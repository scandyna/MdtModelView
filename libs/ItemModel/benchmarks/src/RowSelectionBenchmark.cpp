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
#include "ReadOnlyTableModel.h"
#include "Mdt/ItemModel/RowSelection.h"
#include "Mdt/ItemModel/RowListView.h"
#include <QItemSelection>
#include <QItemSelectionRange>
#include <QAbstractTableModel>
#include <QModelIndex>
#include <QModelIndexList>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

using namespace Mdt::ItemModel;

using RowList = std::vector<int>;

void populateModelWithRowCount(ReadOnlyTableModel & model, int rowCount)
{
  assert( rowCount > 0 );

  using Record = ReadOnlyTableModel::Record;

  ReadOnlyTableModel::Table table;

  for(int row = 0; row < rowCount; ++row){
    Record record{row, "A"};
    table.push_back(record);
  }

  model.setTable(table);
}

RowList rowListFromItemSelection(const QItemSelection & selection) noexcept
{
  RowList rowList;
  const QModelIndexList indexList = selection.indexes();

  for(const QModelIndex & index : indexList){
    rowList.push_back( index.row() );
  }

  std::sort( rowList.begin(), rowList.end() );
  auto last = std::unique( rowList.begin(), rowList.end() );
  rowList.erase( last, rowList.end() );

  return rowList;
}

RowList rowListFromRowSelection(const RowSelection & rowSelection)
{
  RowList rowList;
  RowListView view(rowSelection);

  std::copy( view.cbegin(), view.cend(), std::back_inserter(rowList) );

  return rowList;
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


TEST_CASE("rowListFromItemSelection")
{
  ReadOnlyTableModel model;
  populateModelWithRowCount(model, 10'000);
  QItemSelection itemSelection;
  RowList rowList;

  SECTION("select 10 rows in 1 range")
  {
    addItemRangeToSelection(model, {1,0}, {10,1}, itemSelection);

    BENCHMARK("create row list")
    {
      rowList = rowListFromItemSelection(itemSelection);
    };

    REQUIRE( rowList.size() == 10 );
    REQUIRE( rowList[0] == 1 );
    REQUIRE( rowList[9] == 10 );
  }

  SECTION("select 100 rows in 1 range")
  {
    addItemRangeToSelection(model, {1,0}, {100,1}, itemSelection);

    BENCHMARK("create row list")
    {
      rowList = rowListFromItemSelection(itemSelection);
    };

    REQUIRE( rowList.size() == 100 );
    REQUIRE( rowList[0] == 1 );
    REQUIRE( rowList[99] == 100 );
  }

  SECTION("select 10'000 rows in 1 range")
  {
    addItemRangeToSelection(model, {0,0}, {9'999,1}, itemSelection);

    BENCHMARK("create row list")
    {
      rowList = rowListFromItemSelection(itemSelection);
    };

    REQUIRE( rowList.size() == 10'000 );
    REQUIRE( rowList[0] == 0 );
    REQUIRE( rowList[9'999] == 9'999 );
  }

}

TEST_CASE("rowSelectionFromItemSelection")
{
  ReadOnlyTableModel model;
  populateModelWithRowCount(model, 10'000);
  QItemSelection itemSelection;
  RowSelection rowSelection;
  RowList rowList;

  SECTION("select 10 rows in 1 range")
  {
    addItemRangeToSelection(model, {1,0}, {10,1}, itemSelection);

    BENCHMARK("create row selection")
    {
      rowSelection = RowSelection::fromItemSelection(itemSelection);
    };

    rowList = rowListFromRowSelection(rowSelection);

    REQUIRE( rowList.size() == 10 );
    REQUIRE( rowList[0] == 1 );
    REQUIRE( rowList[9] == 10 );
  }

  SECTION("select 100 rows in 1 range")
  {
    addItemRangeToSelection(model, {1,0}, {100,1}, itemSelection);

    BENCHMARK("create row selection")
    {
      rowSelection = RowSelection::fromItemSelection(itemSelection);
    };

    rowList = rowListFromRowSelection(rowSelection);

    REQUIRE( rowList.size() == 100 );
    REQUIRE( rowList[0] == 1 );
    REQUIRE( rowList[99] == 100 );
  }

  SECTION("select 10'000 rows in 1 range")
  {
    addItemRangeToSelection(model, {0,0}, {9'999,1}, itemSelection);

    BENCHMARK("create row selection")
    {
      rowSelection = RowSelection::fromItemSelection(itemSelection);
    };

    rowList = rowListFromRowSelection(rowSelection);

    REQUIRE( rowList.size() == 10'000 );
    REQUIRE( rowList[0] == 0 );
    REQUIRE( rowList[9'999] == 9'999 );
  }
}

TEST_CASE("iterateOverRowList")
{
  ReadOnlyTableModel model;
  populateModelWithRowCount(model, 10'001);
  QItemSelection itemSelection;
  RowList rowList;
  RowList resultRowList;

  SECTION("10 rows")
  {
    addItemRangeToSelection(model, {1,0}, {10,1}, itemSelection);
    rowList = rowListFromItemSelection(itemSelection);
    resultRowList.reserve( rowList.size() );

    BENCHMARK("iterate over row list")
    {
      resultRowList.clear();
      std::copy( rowList.cbegin(), rowList.cend(), std::back_inserter(resultRowList) );
    };

    REQUIRE( resultRowList.size() == 10 );
    REQUIRE( resultRowList[0] == 1 );
    REQUIRE( resultRowList[9] == 10 );
  }

  SECTION("10'000 rows")
  {
    addItemRangeToSelection(model, {1,0}, {10'000,1}, itemSelection);
    rowList = rowListFromItemSelection(itemSelection);
    resultRowList.reserve( rowList.size() );

    BENCHMARK("iterate over row list")
    {
      resultRowList.clear();
      std::copy( rowList.cbegin(), rowList.cend(), std::back_inserter(resultRowList) );
    };

    REQUIRE( resultRowList.size() == 10'000 );
    REQUIRE( resultRowList[0] == 1 );
    REQUIRE( resultRowList[9'999] == 10'000 );
  }
}

TEST_CASE("iterateOverRowSelection")
{
  ReadOnlyTableModel model;
  populateModelWithRowCount(model, 10'001);
  QItemSelection itemSelection;
  RowSelection rowSelection;
  RowList resultRowList;

  SECTION("10 rows")
  {
    addItemRangeToSelection(model, {1,0}, {10,1}, itemSelection);
    rowSelection = RowSelection::fromItemSelection(itemSelection);
    RowListView rowListView(rowSelection);
    resultRowList.reserve(10);

    BENCHMARK("iterate over row selection")
    {
      resultRowList.clear();
      std::copy( rowListView.cbegin(), rowListView.cend(), std::back_inserter(resultRowList) );
    };

    REQUIRE( resultRowList.size() == 10 );
    REQUIRE( resultRowList[0] == 1 );
    REQUIRE( resultRowList[9] == 10 );
  }

  SECTION("10'000 rows")
  {
    addItemRangeToSelection(model, {1,0}, {10'000,1}, itemSelection);
    rowSelection = RowSelection::fromItemSelection(itemSelection);
    RowListView rowListView(rowSelection);
    resultRowList.reserve(10'000);

    BENCHMARK("iterate over row selection")
    {
      resultRowList.clear();
      std::copy( rowListView.cbegin(), rowListView.cend(), std::back_inserter(resultRowList) );
    };

    REQUIRE( resultRowList.size() == 10'000 );
    REQUIRE( resultRowList[0] == 1 );
    REQUIRE( resultRowList[9'999] == 10'000 );
  }
}

TEST_CASE("createRowListAndUseIt")
{
  ReadOnlyTableModel model;
  populateModelWithRowCount(model, 10'001);
  QItemSelection itemSelection;
  RowList rowList;
  RowList resultRowList;

  SECTION("10 rows in 1 range")
  {
    addItemRangeToSelection(model, {1,0}, {10,1}, itemSelection);
    resultRowList.reserve(10);

    BENCHMARK("create row list and use it")
    {
      rowList = rowListFromItemSelection(itemSelection);
      resultRowList.clear();
      std::copy( rowList.cbegin(), rowList.cend(), std::back_inserter(resultRowList) );
    };

    REQUIRE( resultRowList.size() == 10 );
    REQUIRE( resultRowList[0] == 1 );
    REQUIRE( resultRowList[9] == 10 );
  }

  SECTION("10'000 rows in 1 range")
  {
    addItemRangeToSelection(model, {1,0}, {10'000,1}, itemSelection);
    resultRowList.reserve(10'000);

    BENCHMARK("create row list and use it")
    {
      rowList = rowListFromItemSelection(itemSelection);
      resultRowList.clear();
      std::copy( rowList.cbegin(), rowList.cend(), std::back_inserter(resultRowList) );
    };

    REQUIRE( resultRowList.size() == 10'000 );
    REQUIRE( resultRowList[0] == 1 );
    REQUIRE( resultRowList[9'999] == 10'000 );
  }
}

TEST_CASE("createRowSelectionAndUseIt")
{
  ReadOnlyTableModel model;
  populateModelWithRowCount(model, 10'001);
  QItemSelection itemSelection;
  RowSelection rowSelection;
  RowList resultRowList;

  SECTION("10 rows in 1 range")
  {
    addItemRangeToSelection(model, {1,0}, {10,1}, itemSelection);
    resultRowList.reserve(10);

    BENCHMARK("create a row selection and use it")
    {
      rowSelection = RowSelection::fromItemSelection(itemSelection);
      RowListView rowListView(rowSelection);
      resultRowList.clear();
      std::copy( rowListView.cbegin(), rowListView.cend(), std::back_inserter(resultRowList) );
    };

    REQUIRE( resultRowList.size() == 10 );
    REQUIRE( resultRowList[0] == 1 );
    REQUIRE( resultRowList[9] == 10 );
  }

  SECTION("10'000 rows in 1 range")
  {
    addItemRangeToSelection(model, {1,0}, {10'000,1}, itemSelection);
    resultRowList.reserve(10'000);

    BENCHMARK("create a row selection and use it")
    {
      rowSelection = RowSelection::fromItemSelection(itemSelection);
      RowListView rowListView(rowSelection);
      resultRowList.clear();
      std::copy( rowListView.cbegin(), rowListView.cend(), std::back_inserter(resultRowList) );
    };

    REQUIRE( resultRowList.size() == 10'000 );
    REQUIRE( resultRowList[0] == 1 );
    REQUIRE( resultRowList[9'999] == 10'000 );
  }
}
