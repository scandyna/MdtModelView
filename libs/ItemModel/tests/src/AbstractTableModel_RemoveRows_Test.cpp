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
#include "Mdt/ItemModel/Helpers.h"
#include "RemoveRowsTableModel.h"
#include "Mdt/ItemModel/TestLib/RemoveRowsSignalsSpy.h"
#include <QVariant>
#include <QLatin1String>

using namespace Mdt::ItemModel;
using namespace Mdt::ItemModel::TestLib;

void populateModel(RemoveRowsTableModel & model, const RemoveRowsTableModel::Table & tableData)
{
  model.setTable(tableData);
}

/*
 * Some tests are not repeated here, or only partially
 *
 * See also the other tests for other cases
 */

TEST_CASE("removeRows_support")
{
  RemoveRowsTableModel model;

  SECTION("removing the first row is not supported by this model")
  {
    REQUIRE( !model.supportsRemoveFirstRow() );
  }

  SECTION("removing the last row is not supported by this model")
  {
    REQUIRE( !model.supportsRemoveLastRow() );
  }

  SECTION("remove rows at any valid place is supported by this model")
  {
    REQUIRE( model.supportsRemoveRows() );
  }
}

TEST_CASE("rowAndCountIsValidForRemoveRows")
{
  RemoveRowsTableModel model;

  /*
   * --------
   * |0||1|A|
   * --------
   * |1||2|B|
   * --------
   */
  populateModel(model, {{1,"A"},{2,"B"}});
  REQUIRE( model.rowCount() == 2 );

  SECTION("row < 0 is NOT valid")
  {
    REQUIRE( !model.rowAndCountIsValidForRemoveRows(-1, 1) );
  }

  SECTION("count < 1 is NOT valid")
  {
    REQUIRE( !model.rowAndCountIsValidForRemoveRows(0, 0) );
  }

  SECTION("row 0 count 1 is valid")
  {
    REQUIRE( model.rowAndCountIsValidForRemoveRows(0, 1) );
  }

  SECTION("row 0 count 2 is valid")
  {
    REQUIRE( model.rowAndCountIsValidForRemoveRows(0, 2) );
  }

  SECTION("row 0 count 3 is NOT valid")
  {
    REQUIRE( !model.rowAndCountIsValidForRemoveRows(0, 3) );
  }

  SECTION("row 1 count 1 is valid")
  {
    REQUIRE( model.rowAndCountIsValidForRemoveRows(1, 1) );
  }

  SECTION("row 1 count 2 is NOT valid")
  {
    REQUIRE( model.rowAndCountIsValidForRemoveRows(1, 1) );
  }
}

TEST_CASE("rowAndCountRepresentsRemoveFirstRow")
{
  RemoveRowsTableModel model;

  /*
   * --------
   * |0||1|A|
   * --------
   * |1||2|B|
   * --------
   */
  populateModel(model, {{1,"A"},{2,"B"}});
  REQUIRE( model.rowCount() == 2 );

  SECTION("remove row 0 is removing first row")
  {
    REQUIRE( model.rowAndCountRepresentsRemoveFirstRow(0, 1) );
  }

  SECTION("removing the first 2 rows is NOT removing the first row")
  {
    REQUIRE( !model.rowAndCountRepresentsRemoveFirstRow(0, 2) );
  }

  SECTION("removing row 1 is NOT removing the first row")
  {
    REQUIRE( !model.rowAndCountRepresentsRemoveFirstRow(1, 1) );
  }
}

TEST_CASE("rowAndCountRepresentsRemoveLastRow")
{
  RemoveRowsTableModel model;

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

  SECTION("remove row 0 is NOT removing last row")
  {
    REQUIRE( !model.rowAndCountRepresentsRemoveLastRow(0, 1) );
  }

  SECTION("remove the 2 last rows is NOT removing last row")
  {
    REQUIRE( !model.rowAndCountRepresentsRemoveLastRow(1, 2) );
  }

  SECTION("remove row 2 is removing last row")
  {
    REQUIRE( model.rowAndCountRepresentsRemoveLastRow(2, 1) );
  }
}

TEST_CASE("removeRows")
{
  RemoveRowsTableModel model;

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

  /*
   * --------
   * |0||2|B|
   * --------
   * |1||3|C|
   * --------
   */
  SECTION("remove first row")
  {
    REQUIRE( model.removeRows(0, 1) );

    REQUIRE( model.rowCount() == 2 );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("B") );
    REQUIRE( getModelData(model, 1, 1) == QLatin1String("C") );

    REQUIRE( spy.rowsAboutToBeRemovedCount() == 1 );
    REQUIRE( spy.rowsRemovedCount() == 1 );

    const auto rowsRemoved = spy.rowsRemovedAt(0);
    REQUIRE( spy.rowsAboutToBeRemovedAt(0) == rowsRemoved );
    REQUIRE( !rowsRemoved.parentIndex().isValid() );
    REQUIRE( rowsRemoved.first() == 0 );
    REQUIRE( rowsRemoved.last() == 0 );
  }

  /*
   * --------
   * |0||1|A|
   * --------
   * |1||3|C|
   * --------
   */
  SECTION("remove row 1 (2,B)")
  {
    REQUIRE( model.removeRows(1, 1) );

    REQUIRE( model.rowCount() == 2 );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("A") );
    REQUIRE( getModelData(model, 1, 1) == QLatin1String("C") );

    REQUIRE( spy.rowsAboutToBeRemovedCount() == 1 );
    REQUIRE( spy.rowsRemovedCount() == 1 );

    const auto rowsRemoved = spy.rowsRemovedAt(0);
    REQUIRE( spy.rowsAboutToBeRemovedAt(0) == rowsRemoved );
    REQUIRE( !rowsRemoved.parentIndex().isValid() );
    REQUIRE( rowsRemoved.first() == 1 );
    REQUIRE( rowsRemoved.last() == 1 );
  }

  /*
   * --------
   * |0||1|A|
   * --------
   * |1||2|B|
   * --------
   */
  SECTION("remove last row")
  {
    REQUIRE( model.removeRows(2, 1) );

    REQUIRE( model.rowCount() == 2 );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("A") );
    REQUIRE( getModelData(model, 1, 1) == QLatin1String("B") );

    REQUIRE( spy.rowsAboutToBeRemovedCount() == 1 );
    REQUIRE( spy.rowsRemovedCount() == 1 );

    const auto rowsRemoved = spy.rowsRemovedAt(0);
    REQUIRE( spy.rowsAboutToBeRemovedAt(0) == rowsRemoved );
    REQUIRE( !rowsRemoved.parentIndex().isValid() );
    REQUIRE( rowsRemoved.first() == 2 );
    REQUIRE( rowsRemoved.last() == 2 );
  }

  SECTION("remove all rows")
  {
    REQUIRE( model.removeRows(0, 3) );

    REQUIRE( model.rowCount() == 0 );

    REQUIRE( spy.rowsAboutToBeRemovedCount() == 1 );
    REQUIRE( spy.rowsRemovedCount() == 1 );

    const auto rowsRemoved = spy.rowsRemovedAt(0);
    REQUIRE( spy.rowsAboutToBeRemovedAt(0) == rowsRemoved );
    REQUIRE( !rowsRemoved.parentIndex().isValid() );
    REQUIRE( rowsRemoved.first() == 0 );
    REQUIRE( rowsRemoved.last() == 2 );
  }

  SECTION("try to remove out of bound rows fails")
  {
    REQUIRE( !model.removeRows(1, 3) );

    REQUIRE( model.rowCount() == 3 );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("A") );
    REQUIRE( getModelData(model, 1, 1) == QLatin1String("B") );
    REQUIRE( getModelData(model, 2, 1) == QLatin1String("C") );

    REQUIRE( spy.rowsAboutToBeRemovedCount() == 0 );
    REQUIRE( spy.rowsRemovedCount() == 0 );
  }
}
