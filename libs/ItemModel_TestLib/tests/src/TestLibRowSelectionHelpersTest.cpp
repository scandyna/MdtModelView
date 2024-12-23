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
#include "Mdt/ItemModel/TestLib/RowSelectionHelpers.h"
#include "Mdt/ItemModel/RowSelection.h"
#include "Mdt/ItemModel/RowListView.h"
#include <QStringListModel>
#include <QItemSelectionModel>

using namespace Mdt::ItemModel;
using namespace Mdt::ItemModel::TestLib;


TEST_CASE("makeRowSelectionFromIndexList")
{
  SECTION("empty list")
  {
    RowSelection selection = makeRowSelectionFromIndexList({});

    RowListView listView(selection);

    REQUIRE( listView.cbegin() == listView.cend() );
  }

  SECTION("1,2,3")
  {
    RowSelection selection = makeRowSelectionFromIndexList({1,2,3});

    RowListView listView(selection);

    auto it = listView.cbegin();
    REQUIRE( it != listView.cend() );
    REQUIRE( *it == 1 );
    ++it;
    REQUIRE( it != listView.cend() );
    REQUIRE( *it == 2 );
    ++it;
    REQUIRE( it != listView.cend() );
    REQUIRE( *it == 3 );
    ++it;
    REQUIRE( it == listView.cend() );
  }

  SECTION("1,5,6")
  {
    RowSelection selection = makeRowSelectionFromIndexList({1,5,6});

    RowListView listView(selection);

    auto it = listView.cbegin();
    REQUIRE( it != listView.cend() );
    REQUIRE( *it == 1 );
    ++it;
    REQUIRE( it != listView.cend() );
    REQUIRE( *it == 5 );
    ++it;
    REQUIRE( it != listView.cend() );
    REQUIRE( *it == 6 );
    ++it;
    REQUIRE( it == listView.cend() );
  }
}

TEST_CASE("makeIndexListFromRowSelection")
{
  SECTION("empty list")
  {
    RowSelection selection = makeRowSelectionFromIndexList({});

    auto list = makeIndexListFromRowSelection(selection);

    REQUIRE( list.empty() );
  }

  SECTION("1,2,3")
  {
    RowSelection selection = makeRowSelectionFromIndexList({1,2,3});

    auto list = makeIndexListFromRowSelection(selection);

    REQUIRE( list.size() == 3 );
    REQUIRE( list[0] == 1 );
    REQUIRE( list[1] == 2 );
    REQUIRE( list[2] == 3 );
  }

  SECTION("1,5,6")
  {
    RowSelection selection = makeRowSelectionFromIndexList({1,5,6});

    auto list = makeIndexListFromRowSelection(selection);

    REQUIRE( list.size() == 3 );
    REQUIRE( list[0] == 1 );
    REQUIRE( list[1] == 5 );
    REQUIRE( list[2] == 6 );
  }
}

TEST_CASE("selectRowsAndColumn")
{
  QStringListModel model( QStringList{"A","B","C"} );
  QItemSelectionModel selectionModel(&model);

  selectRowsAndColumn(selectionModel, {0,2}, 0);

  auto rowSelection = RowSelection::fromItemSelection( selectionModel.selection() );
  RowListView rowList(rowSelection);

  auto it = rowList.cbegin();
  REQUIRE( it != rowList.cend() );
  REQUIRE( *it == 0 );
  ++it;
  REQUIRE( it != rowList.cend() );
  REQUIRE( *it == 2 );
  ++it;
  REQUIRE( it == rowList.cend() );
}
