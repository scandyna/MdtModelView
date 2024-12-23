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
#include "Mdt/ItemModel/TestLib/InsertRowsSignalsSpy.h"
#include "Mdt/ItemModel/Helpers.h"
#include <QStringListModel>
#include <QLatin1String>

using namespace Mdt::ItemModel;
using namespace Mdt::ItemModel::TestLib;


TEST_CASE("RowsAboutToBeInsertedSignalRecord")
{
  RowsAboutToBeInsertedSignalRecord record( QModelIndex(), 1, 2 );

  REQUIRE( !record.parentIndex().isValid() );
  REQUIRE( record.first() == 1 );
  REQUIRE( record.last() == 2 );
}

TEST_CASE("RowsInsertedSignalRecord")
{
  RowsInsertedSignalRecord record( QModelIndex(), 1, 2 );

  REQUIRE( !record.parentIndex().isValid() );
  REQUIRE( record.first() == 1 );
  REQUIRE( record.last() == 2 );
}

TEST_CASE("SignalRecordsEquality")
{
  QStringListModel model({QLatin1String("A"),QLatin1String("B")});

  const QModelIndex index1 = model.index(0, 0);

  REQUIRE( index1.isValid() );
  REQUIRE( index1 != QModelIndex() );

  SECTION("AboutToBeInserted(invalid, 1, 2) == Inserted(invalid, 1, 2)")
  {
    REQUIRE(
      RowsAboutToBeInsertedSignalRecord( QModelIndex(), 1, 2 )
      ==
      RowsInsertedSignalRecord( QModelIndex(), 1, 2 )
    );
  }

  SECTION("Inserted(invalid, 1, 2) == AboutToBeInserted(invalid, 1, 2)")
  {
    REQUIRE(
      RowsInsertedSignalRecord( QModelIndex(), 1, 2 )
      ==
      RowsAboutToBeInsertedSignalRecord( QModelIndex(), 1, 2 )
    );
  }

  SECTION("AboutToBeInserted(index1, 1, 2) != Inserted(invalid, 1, 2)")
  {
    REQUIRE(
      RowsAboutToBeInsertedSignalRecord( index1, 1, 2 )
      !=
      RowsInsertedSignalRecord( QModelIndex(), 1, 2 )
    );
  }

  SECTION("AboutToBeInserted(index1, 3, 2) != Inserted(index1, 1, 2)")
  {
    REQUIRE(
      RowsAboutToBeInsertedSignalRecord( index1, 3, 2 )
      !=
      RowsInsertedSignalRecord( index1, 1, 2 )
    );
  }

  SECTION("AboutToBeInserted(index1, 1, 3) != Inserted(index1, 1, 2)")
  {
    REQUIRE(
      RowsAboutToBeInsertedSignalRecord( index1, 1, 3 )
      !=
      RowsInsertedSignalRecord( index1, 1, 2 )
    );
  }
}

TEST_CASE("appendOneRow")
{
  QStringListModel model;
  InsertRowsSignalsSpy spy(model);

  appendRowToModel(model);

  REQUIRE( spy.rowsAboutToBeInsertedCount() == 1 );
  REQUIRE( spy.rowsInsertedCount() == 1 );

  const auto rowsInserted = spy.rowsInsertedAt(0);

  REQUIRE( spy.rowsAboutToBeInsertedAt(0) == rowsInserted );
  REQUIRE( !rowsInserted.parentIndex().isValid() );
  REQUIRE( rowsInserted.first() == 0 );
  REQUIRE( rowsInserted.last() == 0 );
}
