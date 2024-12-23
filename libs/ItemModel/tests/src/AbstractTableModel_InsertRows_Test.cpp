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
#include "InsertRowsTableModel.h"
#include "Mdt/ItemModel/Helpers.h"
#include "Mdt/ItemModel/TestLib/InsertRowsSignalsSpy.h"
#include <QVariant>
#include <QLatin1String>

using namespace Mdt::ItemModel;
using namespace Mdt::ItemModel::TestLib;

void populateModel(InsertRowsTableModel & model, const InsertRowsTableModel::Table & tableData)
{
  model.setTable(tableData);
}


/*
 * Some tests are not repeated here, or only partially
 *
 * See the other tests for other cases
 */

TEST_CASE("columnCount")
{
  InsertRowsTableModel model;

  REQUIRE( model.columnCount() == 2 );
}

TEST_CASE("insertRows_support")
{
  InsertRowsTableModel model;

  SECTION("prepend row is not supported by this model")
  {
    REQUIRE( !model.supportsPrependRow() );
  }

  SECTION("append row is not supported by this model")
  {
    REQUIRE( !model.supportsAppendRow() );
  }

  SECTION("insert rows is supported by this model")
  {
    REQUIRE( model.supportsInsertRows() );
  }
}

TEST_CASE("rowAndCountIsValidForInsertRows")
{
  InsertRowsTableModel model;
  populateModel(model, {{1,"A"}});

  REQUIRE( model.rowCount() == 1 );

  SECTION("a row < 0 is not valid")
  {
    REQUIRE( !model.rowAndCountIsValidForInsertRows(-1, 1) );
  }

  SECTION("row 0 is valid")
  {
    REQUIRE( model.rowAndCountIsValidForInsertRows(0, 1) );
  }

  SECTION("row 1 is valid ( 1 == rowCount() )")
  {
    REQUIRE( model.rowAndCountIsValidForInsertRows(1, 1) );
  }

  SECTION("row 2 is NOT valid ( 2 > rowCount() )")
  {
    REQUIRE( !model.rowAndCountIsValidForInsertRows(2, 1) );
  }

  SECTION("a count < 1 is NOT valid")
  {
    REQUIRE( !model.rowAndCountIsValidForInsertRows(0, 0) );
  }

  SECTION("a count of 1 is valid")
  {
    REQUIRE( model.rowAndCountIsValidForInsertRows(0, 1) );
  }

  SECTION("a count of 2 is valid")
  {
    REQUIRE( model.rowAndCountIsValidForInsertRows(0, 2) );
  }
}

TEST_CASE("rowAndCountRepresentsPrependRow")
{
  InsertRowsTableModel model;

  /*
   * --------
   * |0||1|A|
   * --------
   */
  populateModel(model, {{1,"A"}});
  REQUIRE( model.rowCount() == 1 );

  SECTION("insert 1 row before row 0 is prepend")
  {
    REQUIRE( model.rowAndCountRepresentsPrependRow(0, 1) );
  }

  SECTION("insert 2 rows before row 0 is NOT prepend")
  {
    REQUIRE( !model.rowAndCountRepresentsPrependRow(0, 2) );
  }

  SECTION("insert 1 row before row 1 is NOT prepend")
  {
    REQUIRE( !model.rowAndCountRepresentsPrependRow(1, 1) );
  }
}

TEST_CASE("rowAndCountRepresentsAppendRow")
{
  InsertRowsTableModel model;

  /*
   * --------
   * |0||1|A|
   * --------
   */
  populateModel(model, {{1,"A"}});
  REQUIRE( model.rowCount() == 1 );

  SECTION("insert 1 row before row 0 is NOT an append")
  {
    REQUIRE( !model.rowAndCountRepresentsAppendRow(0, 1) );
  }

  SECTION("insert 1 row before row 1 is an append")
  {
    REQUIRE( model.rowAndCountRepresentsAppendRow(1, 1) );
  }

  SECTION("insert 2 rows before row 1 is an NOT append")
  {
    REQUIRE( !model.rowAndCountRepresentsAppendRow(1, 2) );
  }
}

TEST_CASE("insertRows")
{
  InsertRowsTableModel model;

  /*
   * --------
   * |0||1|A|
   * --------
   * |1||2|B|
   * --------
   */
  populateModel(model, {{1,"A"},{2,"B"}});
  REQUIRE( model.rowCount() == 2 );

  InsertRowsSignalsSpy spy(model);

  /*
   * --------
   * |0|| | |
   * --------
   * |1||1|A|
   * --------
   * |2||2|B|
   * --------
   */
  SECTION("prepend a row")
  {
    REQUIRE( prependRowToModel(model) );

    REQUIRE( model.rowCount() == 3 );
    REQUIRE( getModelData(model, 0, 1).toString().isEmpty() );
    REQUIRE( getModelData(model, 1, 1).toString() == QLatin1String("A") );

    REQUIRE( spy.rowsAboutToBeInsertedCount() == 1 );
    REQUIRE( spy.rowsInsertedCount() == 1 );

    const auto rowsInserted = spy.rowsInsertedAt(0);
    REQUIRE( spy.rowsAboutToBeInsertedAt(0) == rowsInserted );
    REQUIRE( !rowsInserted.parentIndex().isValid() );
    REQUIRE( rowsInserted.first() == 0 );
    REQUIRE( rowsInserted.last() == 0 );
  }

  /*
   * --------
   * |0||1|A|
   * --------
   * |1|| | |
   * --------
   * |2|| | |
   * --------
   * |3||2|B|
   * --------
   */
  SECTION("insert 2 rows before row 1")
  {
    REQUIRE( model.insertRows(1, 2) );

    REQUIRE( model.rowCount() == 4 );
    REQUIRE( getModelData(model, 0, 1).toString() == QLatin1String("A") );
    REQUIRE( getModelData(model, 1, 1).toString().isEmpty() );
    REQUIRE( getModelData(model, 2, 1).toString().isEmpty() );
    REQUIRE( getModelData(model, 3, 1).toString() == QLatin1String("B") );

    REQUIRE( spy.rowsAboutToBeInsertedCount() == 1 );
    REQUIRE( spy.rowsInsertedCount() == 1 );

    const auto rowsInserted = spy.rowsInsertedAt(0);
    REQUIRE( spy.rowsAboutToBeInsertedAt(0) == rowsInserted );
    REQUIRE( !rowsInserted.parentIndex().isValid() );
    REQUIRE( rowsInserted.first() == 1 );
    REQUIRE( rowsInserted.last() == 2 );
  }

  /*
   * --------
   * |0||1|A|
   * --------
   * |1||2|B|
   * --------
   * |2|| | |
   * --------
   */
  SECTION("append a row")
  {
    REQUIRE( appendRowToModel(model) );

    REQUIRE( model.rowCount() == 3 );
    REQUIRE( getModelData(model, 0, 1).toString() == QLatin1String("A") );
    REQUIRE( getModelData(model, 1, 1).toString() == QLatin1String("B") );
    REQUIRE( getModelData(model, 2, 1).toString().isEmpty() );

    REQUIRE( spy.rowsAboutToBeInsertedCount() == 1 );
    REQUIRE( spy.rowsInsertedCount() == 1 );

    const auto rowsInserted = spy.rowsInsertedAt(0);
    REQUIRE( spy.rowsAboutToBeInsertedAt(0) == rowsInserted );
    REQUIRE( !rowsInserted.parentIndex().isValid() );
    REQUIRE( rowsInserted.first() == 2 );
    REQUIRE( rowsInserted.last() == 2 );
  }

  /*
   * --------
   * |0||1|A|
   * --------
   * |1||2|B|
   * --------
   */
  SECTION("trying insert a row at an invalid index fails")
  {
    REQUIRE( !model.insertRows(3, 1) );

    REQUIRE( model.rowCount() == 2 );
    REQUIRE( getModelData(model, 0, 1).toString() == QLatin1String("A") );
    REQUIRE( getModelData(model, 1, 1).toString() == QLatin1String("B") );

    REQUIRE( spy.rowsAboutToBeInsertedCount() == 0 );
    REQUIRE( spy.rowsInsertedCount() == 0 );
  }
}
