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
#include "EditableTableModel.h"
#include "Mdt/ItemModel/Helpers.h"
#include "Mdt/ItemModel/TestLib/DataChangedSignalSpy.h"
#include <QVariant>
#include <QLatin1String>
#include <cassert>

using Mdt::ItemModel::getModelData;
using namespace Mdt::ItemModel::TestLib;

void populateModel(EditableTableModel & model, const EditableTableModel::Table & tableData)
{
  model.setTable(tableData);
}

/*
 * Some tests are not repeated here, or only partially:
 * - construct
 * - rowCount
 * - columnCount
 * - rowIndexIsInRange
 * - columnIndexIsInRange
 * - indexIsValidAndInRange
 * - data
 * - defaultHeaderTest
 * - customHeaderTest
 *
 * See AbstractTableModel_ReadOnly_Test.cpp
 */

TEST_CASE("rowCount")
{
  EditableTableModel model;

  populateModel(model, {{1,"A"}});

  REQUIRE( model.rowCount() == 1 );
  REQUIRE( model.rowCount( model.index(0, 0) ) == 0 );
}

TEST_CASE("columnCount")
{
  EditableTableModel model;

  REQUIRE( model.columnCount() == 2 );
}

TEST_CASE("data")
{
  EditableTableModel model;

  populateModel(model, {{1,"A"}});
  REQUIRE( model.rowCount() == 1 );
  REQUIRE( model.columnCount() == 2 );

  SECTION("get DisplayRole data")
  {
    REQUIRE( getModelData(model, 0, 0) == QVariant(1) );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("A") );
  }

  SECTION("get EditRole data")
  {
    REQUIRE( getModelData(model, 0, 0, Qt::EditRole) == QVariant(1) );
    REQUIRE( getModelData(model, 0, 1, Qt::EditRole) == QLatin1String("A") );
  }
}

TEST_CASE("setData")
{
  EditableTableModel model;
  DataChangedSignalSpy dataChangedSignalSpy(model);

  populateModel(model, {{1,"A"}});
  REQUIRE( model.rowCount() == 1 );
  REQUIRE( model.columnCount() == 2 );

  SECTION("set EditRole data")
  {
    const QModelIndex index = model.index(0, 1);
    REQUIRE( model.setData( index, QString::fromLatin1("Z"), Qt::EditRole ) );

    REQUIRE( dataChangedSignalSpy.count() == 1 );
    REQUIRE( dataChangedSignalSpy.firstTopLeftIndex() == index );
    REQUIRE( dataChangedSignalSpy.firstBottomRightIndex() == index );

    REQUIRE( getModelData(model, 0, 0) == QVariant(1) );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("Z") );
  }

  SECTION("set EditRole data with invalid index")
  {
    REQUIRE( !model.setData(QModelIndex(), 25, Qt::EditRole) );

    REQUIRE( getModelData(model, 0, 0) == QVariant(1) );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("A") );

    REQUIRE( dataChangedSignalSpy.count() == 0 );
  }

  SECTION("set DisplayRole data")
  {
    const QModelIndex index = model.index(0, 1);
    REQUIRE( model.setData( index, QString::fromLatin1("D"), Qt::DisplayRole ) );

    REQUIRE( dataChangedSignalSpy.count() == 1 );
    REQUIRE( dataChangedSignalSpy.firstTopLeftIndex() == index );
    REQUIRE( dataChangedSignalSpy.firstBottomRightIndex() == index );

    REQUIRE( getModelData(model, 0, 0) == QVariant(1) );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("D") );
  }

  SECTION("set TextAlignmentRole data (not supported by this test model)")
  {
    REQUIRE( !setModelData( model, 0, 1, Qt::AlignLeft, Qt::TextAlignmentRole ) );

    REQUIRE( getModelData(model, 0, 0) == QVariant(1) );
    REQUIRE( getModelData(model, 0, 1) == QLatin1String("A") );

    REQUIRE( dataChangedSignalSpy.count() == 0 );
  }
}

TEST_CASE("setRecord")
{
  EditableTableModel model;
  DataChangedSignalSpy dataChangedSignalSpy(model);

  populateModel(model, {{1,"A"}});
  REQUIRE( model.rowCount() == 1 );
  REQUIRE( model.columnCount() == 2 );

  model.setRecord(0, {2,"B"});

  REQUIRE( getModelData(model, 0, 0) == QVariant(2) );
  REQUIRE( getModelData(model, 0, 1) == QLatin1String("B") );

  REQUIRE( dataChangedSignalSpy.count() == 1 );
  REQUIRE( dataChangedSignalSpy.firstTopLeftIndex().row() == 0 );
  REQUIRE( dataChangedSignalSpy.firstTopLeftIndex().column() == 0 );
  REQUIRE( dataChangedSignalSpy.firstBottomRightIndex().row() == 0 );
  REQUIRE( dataChangedSignalSpy.firstBottomRightIndex().column() == 1 );
}
