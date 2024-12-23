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
#include "Mdt/ItemModel/TestLib/DataChangedSignalSpy.h"
#include <QStringListModel>
#include <QLatin1String>

using namespace Mdt::ItemModel::TestLib;


TEST_CASE("Construct")
{
  QStringListModel model({QLatin1String("A")});

  DataChangedSignalSpy spy(model);

  REQUIRE( spy.count() == 0 );
}

TEST_CASE("setData")
{
  QStringListModel model({QLatin1String("A")});
  DataChangedSignalSpy spy(model);

  /*
   * QStringListModel checks if we set a other value than the existing one.
   * If true, it emits dataChanged() with {Qt::DisplayRole, Qt::EditRole} roles.
   */
  const QModelIndex index = model.index(0, 0);
  REQUIRE( model.setData( index, QLatin1String("B") ) );

  REQUIRE( spy.count() == 1 );

  REQUIRE( spy.at(0).topLeftIndex() == index );
  REQUIRE( spy.at(0).bottomRightIndex() == index );
  REQUIRE( spy.at(0).roles().count() == 2 );
  REQUIRE( spy.at(0).roles().at(0) == Qt::DisplayRole );
  REQUIRE( spy.at(0).roles().at(1) == Qt::EditRole );

  REQUIRE( spy.firstTopLeftIndex() == index );
  REQUIRE( spy.firstBottomRightIndex() == index );
  REQUIRE( spy.firstRoles().count() == 2 );
}
