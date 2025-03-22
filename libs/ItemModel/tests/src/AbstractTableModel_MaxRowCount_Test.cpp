// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "ReadOnlyTableModel.h"
#include "MaxRowCountTableModel.h"
#include "Mdt/ItemModel/NumericLimits.h"
#include <limits>


TEST_CASE("maxRowCount_is_intMax_byDefault")
{
  ReadOnlyTableModel model;

  CHECK( model.maxRowCount() == std::numeric_limits<int>::max() );
}

TEST_CASE("maxRowCount")
{
  MaxRowCountTableModel model;

  model.setMaxRowCount(1234);

  CHECK( model.maxRowCount() == 1234 );
}

TEST_CASE("canAddCountRows")
{
  MaxRowCountTableModel model;
  model.setMaxRowCount(5);

  SECTION("empty model")
  {
    REQUIRE( model.rowCount() == 0 );

    CHECK( model.canAddCountRows(1) );
    CHECK( model.canAddCountRows(2) );
    CHECK( model.canAddCountRows(3) );
    CHECK( model.canAddCountRows(4) );
    CHECK( model.canAddCountRows(5) );
    CHECK( !model.canAddCountRows(6) );
    CHECK( !model.canAddCountRows(7) );
    CHECK( !model.canAddCountRows( Mdt::ItemModel::intMax() ) );
  }

  SECTION("model with 2 rows")
  {
    model.setTable({{1,"A"},{2,"B"}});
    REQUIRE( model.rowCount() == 2 );

    CHECK( model.canAddCountRows(1) );
    CHECK( model.canAddCountRows(2) );
    CHECK( model.canAddCountRows(3) );
    CHECK( !model.canAddCountRows(4) );
    CHECK( !model.canAddCountRows( Mdt::ItemModel::intMax() ) );
  }
}

TEST_CASE("rowAndCountIsValidForInsertRows")
{
  MaxRowCountTableModel model;

  model.setMaxRowCount(5);

  SECTION("empty model")
  {
    REQUIRE( model.rowCount() == 0 );

    CHECK( model.rowAndCountIsValidForInsertRows(0, 1) );
    CHECK( model.rowAndCountIsValidForInsertRows(0, 4) );
    CHECK( model.rowAndCountIsValidForInsertRows(0, 5) );
    CHECK( !model.rowAndCountIsValidForInsertRows(0, 6) );
    CHECK( !model.rowAndCountIsValidForInsertRows( 0, Mdt::ItemModel::intMax() ) );
  }

  SECTION("model with 3 rows")
  {
    model.setTable({{1,"A"},{2,"B"},{3,"C"}});
    REQUIRE( model.rowCount() == 3 );

    CHECK( model.rowAndCountIsValidForInsertRows(0, 1) );
    CHECK( model.rowAndCountIsValidForInsertRows(0, 2) );
    CHECK( model.rowAndCountIsValidForInsertRows(2, 1) );
    CHECK( model.rowAndCountIsValidForInsertRows(2, 2) );
    CHECK( model.rowAndCountIsValidForInsertRows(3, 2) );
    CHECK( !model.rowAndCountIsValidForInsertRows(0, 3) );
    CHECK( !model.rowAndCountIsValidForInsertRows(3, 3) );
    CHECK( !model.rowAndCountIsValidForInsertRows( 0, Mdt::ItemModel::intMax() ) );
  }
}
