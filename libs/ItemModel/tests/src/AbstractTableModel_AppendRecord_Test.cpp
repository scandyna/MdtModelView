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
#include "AppendRecordTableModel.h"
#include "Mdt/ItemModel/Helpers.h"
#include "Mdt/ItemModel/TestLib/InsertRowsSignalsSpy.h"
#include <QVariant>
#include <QLatin1String>

using namespace Mdt::ItemModel;
using namespace Mdt::ItemModel::TestLib;

/*
 * Some tests are not repeated here, or only partially
 *
 * Most of the insert tests are done in AbstractTableModel_InsertRows_Test
 *
 * See also the other tests for other cases
 */


TEST_CASE("appendRecord")
{
  AppendRecordTableModel model;

  model.setTable({{1,"A"}});
  REQUIRE( model.rowCount() == 1 );

  /*
   * Append a record
   * -----
   * |1|A|
   * -----
   * |2|B|
   * -----
   */
  InsertRowsSignalsSpy spy(model);

  model.appendRecord({2,"B"});

  REQUIRE( model.rowCount() == 2 );
  REQUIRE( getModelData(model, 0, 1) == QLatin1String("A") );
  REQUIRE( getModelData(model, 1, 1) == QLatin1String("B") );

  REQUIRE( spy.rowsAboutToBeInsertedCount() == 1 );
  REQUIRE( spy.rowsInsertedCount() == 1 );

  const auto rowsInserted = spy.rowsInsertedAt(0);
  REQUIRE( spy.rowsAboutToBeInsertedAt(0) == rowsInserted );
  REQUIRE( !rowsInserted.parentIndex().isValid() );
  REQUIRE( rowsInserted.first() == 1 );
  REQUIRE( rowsInserted.last() == 1 );
}
