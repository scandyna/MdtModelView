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
#include "RemoveFirstRowTableModel.h"
#include "Mdt/ItemModel/Helpers.h"
#include "Mdt/ItemModel/TestLib/RemoveRowsSignalsSpy.h"
#include <QVariant>
#include <QLatin1String>

using namespace Mdt::ItemModel;
using namespace Mdt::ItemModel::TestLib;

void populateModel(RemoveFirstRowTableModel & model, const RemoveFirstRowTableModel::Table & tableData)
{
  model.setTable(tableData);
}

/*
 * Some tests are not repeated here, or only partially
 *
 * Most of the remove tests are done in AbstractTableModel_RemoveRows_Test
 *
 * See also the other tests for other cases
 */

TEST_CASE("removeRows_support")
{
  RemoveFirstRowTableModel model;

  SECTION("removing the first row is supported by this model")
  {
    REQUIRE( model.supportsRemoveFirstRow() );
  }

  SECTION("removing the last row is not supported by this model")
  {
    REQUIRE( !model.supportsRemoveLastRow() );
  }

  SECTION("remove rows at any valid place is not supported by this model")
  {
    REQUIRE( !model.supportsRemoveRows() );
  }
}

TEST_CASE("removeFirstRow")
{
  RemoveFirstRowTableModel model;

  /*
   * --------
   * |0||1|A|
   * --------
   * |1||2|B|
   * --------
   */
  populateModel(model, {{1,"A"},{2,"B"}});
  REQUIRE( model.rowCount() == 2 );

  RemoveRowsSignalsSpy spy(model);

  REQUIRE( removeFirstRowFromModel(model) );

  REQUIRE( model.rowCount() == 1 );
  REQUIRE( getModelData(model, 0, 1) == QLatin1String("B") );

  REQUIRE( spy.rowsAboutToBeRemovedCount() == 1 );
  REQUIRE( spy.rowsRemovedCount() == 1 );

  const auto rowsRemoved = spy.rowsRemovedAt(0);
  REQUIRE( spy.rowsAboutToBeRemovedAt(0) == rowsRemoved );
  REQUIRE( !rowsRemoved.parentIndex().isValid() );
  REQUIRE( rowsRemoved.first() == 0 );
  REQUIRE( rowsRemoved.last() == 0 );
}

TEST_CASE("removeRow_NotSupportedCases")
{
  RemoveFirstRowTableModel model;

  /*
   * --------
   * |0||1|A|
   * --------
   * |1||2|B|
   * --------
   * |2||3|C|
   * --------
   */
  populateModel(model, {{1,"A"},{2,"B"},{3,"C"}});
  REQUIRE( model.rowCount() == 3 );

  RemoveRowsSignalsSpy spy(model);

  SECTION("remove the last row is not supported")
  {
    REQUIRE( !removeLastRowFromModel(model) );

    REQUIRE( model.rowCount() == 3 );

    REQUIRE( spy.rowsAboutToBeRemovedCount() == 0 );
    REQUIRE( spy.rowsRemovedCount() == 0 );
  }

  SECTION("remove the 2 first rows is not supported")
  {
    REQUIRE( !model.removeRows(0, 2) );

    REQUIRE( model.rowCount() == 3 );

    REQUIRE( spy.rowsAboutToBeRemovedCount() == 0 );
    REQUIRE( spy.rowsRemovedCount() == 0 );
  }
}
