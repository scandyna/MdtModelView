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
#include "Mdt/ItemModel/TestLib/RemoveRowsSignalsSpy.h"
#include "Mdt/ItemModel/Helpers.h"
#include <QStringListModel>
#include <QLatin1String>

using namespace Mdt::ItemModel;
using namespace Mdt::ItemModel::TestLib;


TEST_CASE("RowsAboutToBeRemovedSignalRecord")
{
  RowsAboutToBeRemovedSignalRecord record( QModelIndex(), 1, 2 );

  REQUIRE( !record.parentIndex().isValid() );
  REQUIRE( record.first() == 1 );
  REQUIRE( record.last() == 2 );
}

TEST_CASE("RowsRemovedSignalRecord")
{
  RowsRemovedSignalRecord record( QModelIndex(), 1, 2 );

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

  SECTION("AboutToBeRemoved(invalid, 1, 2) == Removed(invalid, 1, 2)")
  {
    REQUIRE(
      RowsAboutToBeRemovedSignalRecord( QModelIndex(), 1, 2 )
      ==
      RowsRemovedSignalRecord( QModelIndex(), 1, 2 )
    );
  }

  SECTION("Removed(invalid, 1, 2) == AboutToBeRemoved(invalid, 1, 2)")
  {
    REQUIRE(
      RowsRemovedSignalRecord( QModelIndex(), 1, 2 )
      ==
      RowsAboutToBeRemovedSignalRecord( QModelIndex(), 1, 2 )
    );
  }

  SECTION("AboutToBeRemoved(index1, 1, 2) != Removed(invalid, 1, 2)")
  {
    REQUIRE(
      RowsAboutToBeRemovedSignalRecord( index1, 1, 2 )
      !=
      RowsRemovedSignalRecord( QModelIndex(), 1, 2 )
    );
  }

  SECTION("AboutToBeRemoved(index1, 3, 2) != Removed(index1, 1, 2)")
  {
    REQUIRE(
      RowsAboutToBeRemovedSignalRecord( index1, 3, 2 )
      !=
      RowsRemovedSignalRecord( index1, 1, 2 )
    );
  }

  SECTION("AboutToBeRemoved(index1, 1, 3) != Removed(index1, 1, 2)")
  {
    REQUIRE(
      RowsAboutToBeRemovedSignalRecord( index1, 1, 3 )
      !=
      RowsRemovedSignalRecord( index1, 1, 2 )
    );
  }
}

TEST_CASE("removeOneRow")
{
  QStringListModel model({QLatin1String("A"),QLatin1String("B")});

  RemoveRowsSignalsSpy spy(model);

  removeLastRowFromModel(model);

  REQUIRE( spy.rowsAboutToBeRemovedCount() == 1 );
  REQUIRE( spy.rowsRemovedCount() == 1 );

  const auto rowsRemoved = spy.rowsRemovedAt(0);

  REQUIRE( spy.rowsAboutToBeRemovedAt(0) == rowsRemoved );
  REQUIRE( !rowsRemoved.parentIndex().isValid() );
  REQUIRE( rowsRemoved.first() == 1 );
  REQUIRE( rowsRemoved.last() == 1 );
}
