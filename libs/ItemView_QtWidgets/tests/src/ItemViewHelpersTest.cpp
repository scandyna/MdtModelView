// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ItemViewHelpersTest.h"
#include "RemoveRowsTableModel.h"
#include "Mdt/ItemView/Helpers.h"
#include <QTableView>
#include <cassert>

#include <QItemSelectionModel>
#include <QItemSelection>

using namespace Mdt::ItemView;

/// \todo Should go to some test lib
void waitWhileWidgetIsVisible(QWidget & widget)
{
  while( widget.isVisible() ){
    QTest::qWait(200);
  }
}

void populateModel(RemoveRowsTableModel & model, const RemoveRowsTableModel::Table & tableData)
{
  model.setTable(tableData);
}

void selectItem(QAbstractItemView & view, int row, int column)
{
  assert( view.selectionModel() != nullptr );

  const auto *model = view.model();
  assert( model != nullptr );

  assert( row >= 0 );
  assert( row < model->rowCount() );
  assert( column >= 0 );
  assert( column < model->columnCount() );

  QModelIndex index = model->index(row, column);

  view.selectionModel()->select(index, QItemSelectionModel::Select);
}

/*
 * Most of the tests are done in ItemModelHelpersTest
 */

void ItemViewHelpersTest::removeSelectedRows_SelectAll()
{
  QTableView view;
  RemoveRowsTableModel model;

  populateModel(model, {{1,"A"},{2,"B"},{3,"C"},{4,"D"}});
  view.setModel(&model);
  view.show();
  QCOMPARE( model.rowCount(), 4 );

  view.selectAll();

  QVERIFY( removeSelectedRows(view) );

  QCOMPARE( model.rowCount(), 0 );
}

QTEST_MAIN(ItemViewHelpersTest)
