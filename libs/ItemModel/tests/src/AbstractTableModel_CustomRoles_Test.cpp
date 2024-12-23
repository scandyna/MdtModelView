// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "CustomRolesTableModel.h"
#include "Mdt/ItemModel/Helpers.h"
#include "Mdt/ItemModel/TestLib/DataChangedSignalSpy.h"

using Mdt::ItemModel::getModelData;
using namespace Mdt::ItemModel::TestLib;


TEST_CASE("data")
{
  CustomRolesTableModel model;

  model.setTable({{1,"A"}});
  REQUIRE( model.rowCount() == 1 );
  REQUIRE( model.columnCount() == 2 );

  SECTION("get DisplayRole data")
  {
    REQUIRE( getModelData(model, 0, 0) == QVariant(1) );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("A") );
  }

  SECTION("get DisplayRole data with invalid index")
  {
    REQUIRE( model.data( QModelIndex() ).isNull() );
  }

  SECTION("get ToolTipRole data")
  {
    REQUIRE( getModelData(model, 0, 0, Qt::ToolTipRole).toString() == "Column 0 tool tip" );
  }
}

TEST_CASE("setData")
{
  CustomRolesTableModel model;
  DataChangedSignalSpy dataChangedSignalSpy(model);

  model.setTable({{1,"A"}});
  REQUIRE( model.rowCount() == 1 );
  REQUIRE( model.columnCount() == 2 );

  SECTION("set edit role data is not supported")
  {
    REQUIRE( !setModelData( model, 0, 1, "some data", Qt::EditRole) );

    REQUIRE( getModelData(model, 0, 0) == QVariant(1) );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("A") );

    REQUIRE( dataChangedSignalSpy.count() == 0 );
  }

  SECTION("set ToolTipRole data")
  {
    const QModelIndex index = model.index(0, 0);
    REQUIRE( model.setData( index, "My custom tool tip", Qt::ToolTipRole ) );

    REQUIRE( dataChangedSignalSpy.count() == 1 );
    REQUIRE( dataChangedSignalSpy.firstTopLeftIndex() == index );
    REQUIRE( dataChangedSignalSpy.firstBottomRightIndex() == index );

    REQUIRE( getModelData(model, 0, 0) == QVariant(1) );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("A") );
    REQUIRE( getModelData(model, 0, 0, Qt::ToolTipRole).toString() == "My custom tool tip" );
  }
}
