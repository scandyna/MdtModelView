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
#include "ReadOnlyTableModel.h"
#include "DefaultHeaderTableModel.h"
#include "CustomHeaderTableModel.h"
#include "Mdt/ItemModel/Helpers.h"
#include "Mdt/ItemModel/TestLib/DataChangedSignalSpy.h"
#include <QVariant>
#include <QLatin1String>
#include <cassert>

using Mdt::ItemModel::getModelData;
using namespace Mdt::ItemModel::TestLib;

void populateModel(ReadOnlyTableModel & model, const ReadOnlyTableModel::Table & tableData)
{
  model.setTable(tableData);
}


TEST_CASE("construct")
{
  SECTION("stack allocation")
  {
    ReadOnlyTableModel model;
    REQUIRE( model.columnCount() == 2 );
  }

  SECTION("dynamic alloc with parent as guard")
  {
    QObject parent;
    auto *model = new ReadOnlyTableModel(&parent);
    REQUIRE( model->columnCount() == 2 );
  }
}

TEST_CASE("rowCount")
{
  ReadOnlyTableModel model;

  SECTION("a default constructed model has 0 row")
  {
    REQUIRE( model.rowCount() == 0 );
  }

  SECTION("1 row")
  {
    populateModel(model, {{1,"A"}});

    REQUIRE( model.rowCount() == 1 );
    REQUIRE( model.rowCount( model.index(0, 0) ) == 0 );
  }
}

TEST_CASE("columnCount")
{
  ReadOnlyTableModel model;

  SECTION("a default constructed model has 2 columns")
  {
    REQUIRE( model.columnCount() == 2 );
  }

  SECTION("1 row - still 2 columns and 0 if parent is valid")
  {
    populateModel(model, {{1,"A"}});

    REQUIRE( model.columnCount() == 2 );
    REQUIRE( model.columnCount( model.index(0, 0) ) == 0 );
  }
}

TEST_CASE("rowIndexIsInRange")
{
  ReadOnlyTableModel model;
  populateModel(model, {{1,"A"}});

  SECTION("-1 is not in range")
  {
    REQUIRE( !model.rowIndexIsInRange(-1) );
  }

  SECTION("0 is in range")
  {
    REQUIRE( model.rowIndexIsInRange(0) );
  }

  SECTION("1 is not in range")
  {
    REQUIRE( !model.rowIndexIsInRange(1) );
  }
}

TEST_CASE("columnIndexIsInRange")
{
  ReadOnlyTableModel model;

  SECTION("-1 is not in range")
  {
    REQUIRE( !model.columnIndexIsInRange(-1) );
  }

  SECTION("0 is in range")
  {
    REQUIRE( model.columnIndexIsInRange(0) );
  }

  SECTION("1 is in range")
  {
    REQUIRE( model.columnIndexIsInRange(1) );
  }

  SECTION("2 is not in range")
  {
    REQUIRE( !model.columnIndexIsInRange(2) );
  }
}

TEST_CASE("indexIsValidAndInRange")
{
  ReadOnlyTableModel model;
  populateModel(model, {{1,"A"}});

  SECTION("default constructed index (invalid)")
  {
    QModelIndex index;

    REQUIRE( !model.indexIsValidAndInRange(index) );
  }

  SECTION("valid index")
  {
    const QModelIndex index = model.index(0, 1);
    REQUIRE( index.isValid() );

    REQUIRE( model.indexIsValidAndInRange(index) );
  }

  /*
   * It could be good to check with out of bound index
   * I don't know how to do that in a reasonable way
   * QAbstractTableModel::index() does bound checking
   * using hasIndex()
   *
   * QAbstractItemModelTester seems also not to check that.
   *
   * We should rely on a valid implementation here
   * Note: rowIndexIsInRange() and columnIndexIsInRange() are tested above
   */
}

TEST_CASE("data")
{
  ReadOnlyTableModel model;

  populateModel(model, {{1,"A"}});
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

  SECTION("get FontRole data")
  {
    REQUIRE( getModelData(model, 0, 0, Qt::FontRole).isNull() );
    REQUIRE( getModelData(model, 0, 1, Qt::FontRole).isNull() );
  }
}

TEST_CASE("setData")
{
  ReadOnlyTableModel model;

  populateModel(model, {{1,"A"}});
  REQUIRE( model.rowCount() == 1 );
  REQUIRE( model.columnCount() == 2 );

  DataChangedSignalSpy dataChangedSignalSpy(model);

  REQUIRE( !setModelData( model, 0, 1, QString::fromLatin1("Z") ) );

  REQUIRE( getModelData(model, 0, 0) == QVariant(1) );
  REQUIRE( getModelData(model, 0, 1) == QLatin1String("A") );

  REQUIRE( dataChangedSignalSpy.count() == 0 );
}

TEST_CASE("insertRows_support")
{
  ReadOnlyTableModel model;

  SECTION("prepend row is not supported by this model")
  {
    REQUIRE( !model.supportsPrependRow() );
  }

  SECTION("append row is not supported by this model")
  {
    REQUIRE( !model.supportsAppendRow() );
  }

  SECTION("insert rows is not supported by this model")
  {
    REQUIRE( !model.supportsInsertRows() );
  }
}

TEST_CASE("removeRows_support")
{
  ReadOnlyTableModel model;

  SECTION("removing the first row is not supported by this model")
  {
    REQUIRE( !model.supportsRemoveFirstRow() );
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

TEST_CASE("defaultHeaderTest")
{
  DefaultHeaderTableModel model;
  REQUIRE( model.rowCount() == 2 );
  REQUIRE( model.columnCount() == 3 );

  SECTION("horizontal header")
  {
    REQUIRE( model.headerData(0, Qt::Horizontal) == QVariant(1) );
    REQUIRE( model.headerData(1, Qt::Horizontal) == QVariant(2) );
    REQUIRE( model.headerData(2, Qt::Horizontal) == QVariant(3) );
  }

  SECTION("horizontal header DecorationRole")
  {
    REQUIRE( model.headerData(0, Qt::Horizontal, Qt::DecorationRole).isNull() );
  }

  SECTION("horizontal header out of range")
  {
    REQUIRE( model.headerData(3, Qt::Horizontal).isNull() );
  }

  SECTION("vertical header")
  {
    REQUIRE( model.headerData(0, Qt::Vertical) == QVariant(1) );
    REQUIRE( model.headerData(1, Qt::Vertical) == QVariant(2) );
  }

  SECTION("vertical header DecorationRole")
  {
    REQUIRE( model.headerData(0, Qt::Vertical, Qt::DecorationRole).isNull() );
  }

  SECTION("vertical header out of range")
  {
    REQUIRE( model.headerData(3, Qt::Vertical).isNull() );
  }
}

TEST_CASE("customHeaderTest")
{
  CustomHeaderTableModel model;
  REQUIRE( model.rowCount() == 2 );
  REQUIRE( model.columnCount() == 3 );

  SECTION("horizontal header")
  {
    REQUIRE( model.headerData(0, Qt::Horizontal) == QLatin1String("A") );
    REQUIRE( model.headerData(1, Qt::Horizontal) == QLatin1String("B") );
    REQUIRE( model.headerData(2, Qt::Horizontal) == QLatin1String("C") );
  }

  SECTION("vertical header")
  {
    REQUIRE( model.headerData(0, Qt::Vertical) == QVariant(10) );
    REQUIRE( model.headerData(1, Qt::Vertical) == QVariant(11) );
  }
}
