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
#include "AppendRowTableModel.h"
#include "Mdt/ItemModel/Helpers.h"
#include "Mdt/ItemModel/TestLib/InsertRowsSignalsSpy.h"
#include <QVariant>
#include <QLatin1String>

using namespace Mdt::ItemModel;
using namespace Mdt::ItemModel::TestLib;

void populateModel(AppendRowTableModel & model, const AppendRowTableModel::Table & tableData)
{
  model.setTable(tableData);
}


/*
 * Some tests are not repeated here, or only partially
 *
 * Most of the insert tests are done in AbstractTableModel_InsertRows_Test
 *
 * See also the other tests for other cases
 */

TEST_CASE("columnCount")
{
  AppendRowTableModel model;

  REQUIRE( model.columnCount() == 2 );
}

TEST_CASE("insertRows_support")
{
  AppendRowTableModel model;

  SECTION("prepend row is not supported by this model")
  {
    REQUIRE( !model.supportsPrependRow() );
  }

  SECTION("append row is supported by this model")
  {
    REQUIRE( model.supportsAppendRow() );
  }

  SECTION("insert rows is not supported by this model")
  {
    REQUIRE( !model.supportsInsertRows() );
  }
}

TEST_CASE("appendRow")
{
  AppendRowTableModel model;

  populateModel(model, {{1,"A"}});
  REQUIRE( model.rowCount() == 1 );

  /*
   * Append a row
   * -----
   * |1|A|
   * -----
   * | | |
   * -----
   */
  InsertRowsSignalsSpy spy(model);

  REQUIRE( appendRowToModel(model) );

  REQUIRE( model.rowCount() == 2 );
  REQUIRE( getModelData(model, 0, 1) == QLatin1String("A") );
  REQUIRE( getModelData(model, 1, 1).toString().isEmpty() );

  REQUIRE( spy.rowsAboutToBeInsertedCount() == 1 );
  REQUIRE( spy.rowsInsertedCount() == 1 );

  const auto rowsInserted = spy.rowsInsertedAt(0);
  REQUIRE( spy.rowsAboutToBeInsertedAt(0) == rowsInserted );
  REQUIRE( !rowsInserted.parentIndex().isValid() );
  REQUIRE( rowsInserted.first() == 1 );
  REQUIRE( rowsInserted.last() == 1 );
}

TEST_CASE("insertRow_NotSupportedCase")
{
  AppendRowTableModel model;

  /*
   * -----
   * |1|A|
   * -----
   */
  populateModel(model, {{1,"A"}});
  REQUIRE( model.rowCount() == 1 );

  InsertRowsSignalsSpy spy(model);

  SECTION("prepending a row is not supported")
  {
    REQUIRE( !prependRowToModel(model) );

    REQUIRE( model.rowCount() == 1 );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("A") );

    REQUIRE( spy.rowsAboutToBeInsertedCount() == 0 );
    REQUIRE( spy.rowsInsertedCount() == 0 );
  }

  SECTION("inserting 2 rows at the end is not supported")
  {
    REQUIRE( !model.insertRows(1, 2) );

    REQUIRE( model.rowCount() == 1 );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("A") );

    REQUIRE( spy.rowsAboutToBeInsertedCount() == 0 );
    REQUIRE( spy.rowsInsertedCount() == 0 );
  }
}
