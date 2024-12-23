// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ItemSelectionModelWidgetsTest.h"
#include "ReadOnlyTableModel.h"
#include <QModelIndex>
#include <QLatin1String>
#include <QTableView>
#include <QListView>
#include <QTreeView>
#include <QWidget>
#include <QWindow>
#include <QDebug>
#include <cassert>

using namespace Mdt::ItemModel;


/*
 * Workaround for qWaitForWindowActive() failing in the CI
 * See https://gitlab.com/scandyna/docker-images-ubuntu/-/issues/5
 */
bool waitForWindowActive(QWidget & widget)
{
  QWindow *window = widget.window()->windowHandle();
  if(window == nullptr){
    qWarning() << "widget has no window handle";
  }

  if( QTest::qWaitForWindowActive(&widget) ){
    return true;
  }

  QTest::qWait(100);

  return true;
}


void setViewCurrentIndexToRowAndColumn(QAbstractItemView & view, int row, int column)
{
  assert( view.model() != nullptr );

  QModelIndex index = view.model()->index(row, column);
  assert( index.isValid() );

  view.setCurrentIndex(index);
}

void selectRowAndColumn(QAbstractItemView & view, int row, int column, QItemSelectionModel::SelectionFlags command = QItemSelectionModel::ClearAndSelect)
{
  assert(view.selectionModel() != nullptr);
  auto selectionModel = view.selectionModel();
  assert(selectionModel->model() != nullptr);

  QModelIndex index = selectionModel->model()->index(row, column);
  assert( index.isValid() );

  selectionModel->select(index, command);
}

bool indexByRowAndColumnIsSelected(QItemSelectionModel & selectionModel, int row, int column)
{
  assert(selectionModel.model() != nullptr);

  QModelIndex index = selectionModel.model()->index(row, column);
  assert( index.isValid() );

  return selectionModel.isSelected(index);
}


ItemSelectionModelWidgetsTest::ItemSelectionModelWidgetsTest(QObject *parent)
 : QObject(parent)
{
}

void ItemSelectionModelWidgetsTest::initTestCase()
{
}

void ItemSelectionModelWidgetsTest::cleanup()
{
}

/*
 * This test is a sequence of some scenarios.
 * Those have been written after playing with a view,
 * with a lot of debug outputs, reading the docs and some of the Qt source code.
 *
 * \sa https://doc.qt.io/qt-6/model-view-programming.html#handling-selections-in-item-views
 * \sa https://doc.qt.io/qt-6/qabstractitemview.html#details
 */

void ItemSelectionModelWidgetsTest::tableViewTest()
{
  ReadOnlyTableModel model;
  model.setTable({{1,"A"},{2,"B"},{3,"C"}});

  QTableView view;
  view.setModel(&model);

  ItemSelectionModel selectionModel(&model);
  view.setSelectionModel(&selectionModel);

  /*
   * We use the default selection mode and behaviour of the view
   * Because the test can depend on those,
   * check that they are the expected ones.
   */
  QVERIFY( view.selectionBehavior() == QAbstractItemView::SelectItems );
  QVERIFY( view.selectionMode() == QAbstractItemView::ExtendedSelection );

  QVERIFY( selectionModel.changeCurrentRowIsAllowed() );

  /*
   * Wait until the view is visible and has called setCurrentIndex(0,0)
   */
  view.show();
  QApplication::setActiveWindow(&view);
  QVERIFY( waitForWindowActive(view) );

  /*
   * Initial state:
   * The view has called setCurrentIndex(0,0, NoUpdate).
   * The item at index 0,0 is current (but not selected).
   */
  QCOMPARE( selectionModel.currentIndex().row(), 0 );

  /*
   * Now disallow changing current row
   */
  selectionModel.setChangeCurrentRowAllowed(false);

  /*
   * The user can navigate to another column in the same row
   *
   * The selection model should accept this change.
   *
   * We use setCurrentIndex() on the view,
   * which will change the current index and select it.
   */
  setViewCurrentIndexToRowAndColumn(view, 0, 1);
  QCOMPARE( selectionModel.currentIndex().row(), 0 );
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 0, 1) );

  /*
   * State: index 0,1 is current and selected
   *
   * The user wants to navigate to another row
   *
   * We tell the view to got to index 1,0
   * The selection model does not allow this row change
   * Index 0,1 is still current and selected
   *
   * Then the view calls select(1,0,ClearAndSelect).
   * The selection must not change.
   */
  setViewCurrentIndexToRowAndColumn(view, 1, 0);
  QCOMPARE( selectionModel.currentIndex().row(), 0 );
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 0, 1) );
  selectRowAndColumn(view, 1, 0, QItemSelectionModel::ClearAndSelect);
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 0, 1) );
  QVERIFY( !indexByRowAndColumnIsSelected(selectionModel, 1, 0) );

  /*
   * Now allow changing row
   *
   * Nothing more happens for the moment
   */
  selectionModel.setChangeCurrentRowAllowed(true);
  QCOMPARE( selectionModel.currentIndex().row(), 0 );
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 0, 1) );
  QVERIFY( !indexByRowAndColumnIsSelected(selectionModel, 1, 0) );

  /*
   * State: index 0,1 is current and selected
   *
   * The user wants to navigate to another row
   *
   * We tell the view to got to index 1,0
   *
   * The selection model sets the correct index to 1,0 (NOT 1,1)
   *
   * The item at index 1,0 is current and selected
   * The item at index 1,1 is NOT selected
   * The previously selected item at index 0,1 is not selected anymore
   */
  setViewCurrentIndexToRowAndColumn(view, 1, 0);
  QCOMPARE( selectionModel.currentIndex().row(), 1 );
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 1, 0) );
  QVERIFY( !indexByRowAndColumnIsSelected(selectionModel, 1, 1) );
  QVERIFY( !indexByRowAndColumnIsSelected(selectionModel, 0, 1) );
}

void ItemSelectionModelWidgetsTest::tableViewSelectRowsTest()
{
  ReadOnlyTableModel model;
  model.setTable({{1,"A"},{2,"B"},{3,"C"}});

  QTableView view;
  view.setModel(&model);

  ItemSelectionModel selectionModel(&model);
  view.setSelectionModel(&selectionModel);

  /*
   * Here we test using SelectRows behaviour and default mode
   */
  view.setSelectionBehavior(QAbstractItemView::SelectRows);
  QVERIFY( view.selectionBehavior() == QAbstractItemView::SelectRows );
  QVERIFY( view.selectionMode() == QAbstractItemView::ExtendedSelection );

  /*
   * Wait until the view is visible and has called setCurrentIndex(0,0)
   */
  view.show();
  QApplication::setActiveWindow(&view);
  QVERIFY( waitForWindowActive(view) );

  /*
   * Initial state:
   * currentIndex is invalid.
   * The view has called setCurrentIndex(0,0).
   *
   * The item at index 0,0 is current (but not selected).
   */
  QCOMPARE( selectionModel.currentIndex().row(), 0 );

  /*
   * Now disallow changing current row
   */
  selectionModel.setChangeCurrentRowAllowed(false);

  /*
   * The user can navigate to another column in the same row
   *
   * The selection model should accept this change.
   *
   * We use setCurrentIndex() on the view,
   * which will change the current index.
   *
   * This time, setCurrentIndex(0,1,Rows|ClearAndSelect) will be called.
   * This calls select(0,1-0,1,Rows|ClearAndSelect),
   * wich is a single item selection, but with a command telling to select the complete row.
   */
  setViewCurrentIndexToRowAndColumn(view, 0, 1);
  QCOMPARE( selectionModel.currentIndex().row(), 0 );
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 0, 1) );

  /*
   * State: index 0,1 is current and selected
   *
   * The user wants to navigate to another row
   *
   * We tell the view to got to index 1,0
   * The selection model does not allow this row change
   * Index 0,1 is still current and selected
   *
   * Then the view calls select(1,0,Rows|ClearAndSelect).
   * The selection must not change.
   */
  setViewCurrentIndexToRowAndColumn(view, 1, 0);
  QCOMPARE( selectionModel.currentIndex().row(), 0 );
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 0, 1) );
  selectRowAndColumn(view, 1, 0, QItemSelectionModel::Rows|QItemSelectionModel::ClearAndSelect);
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 0, 1) );
  QVERIFY( !indexByRowAndColumnIsSelected(selectionModel, 1, 0) );
  QVERIFY( !indexByRowAndColumnIsSelected(selectionModel, 1, 1) );

  /*
   * Now allow changing row
   *
   * Nothing more happens for the moment
   */
  selectionModel.setChangeCurrentRowAllowed(true);
  QCOMPARE( selectionModel.currentIndex().row(), 0 );
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 0, 1) );

  /*
   * State: index 0,1 is current and selected
   *
   * The user wants to navigate to another row
   *
   * We tell the view to got to index 1,0
   *
   * The selection model sets the correct index to 1,0
   *
   * The item at index 1,0 is current and selected
   * The previously selected item at index 0,1 is not selected anymore
   */
  setViewCurrentIndexToRowAndColumn(view, 1, 0);
  QCOMPARE( selectionModel.currentIndex().row(), 1 );
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 1, 0) );
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 1, 1) );
  QVERIFY( !indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
  QVERIFY( !indexByRowAndColumnIsSelected(selectionModel, 0, 1) );
}

void ItemSelectionModelWidgetsTest::listViewTest()
{
  ReadOnlyTableModel model;
  model.setTable({{1,"A"},{2,"B"},{3,"C"}});

  QListView view;
  view.setModel(&model);

  ItemSelectionModel selectionModel(&model);
  view.setSelectionModel(&selectionModel);

  /*
   * We use the default selection mode and behaviour of the view
   * Because the test can depend on those,
   * check that they are the expected ones.
   */
  QVERIFY( view.selectionBehavior() == QAbstractItemView::SelectItems );
  QVERIFY( view.selectionMode() == QAbstractItemView::SingleSelection );

  /*
   * Wait until the view is visible and has called setCurrentIndex(0,0)
   */
  view.show();
  QApplication::setActiveWindow(&view);
  QVERIFY( waitForWindowActive(view) );

  /*
   * Initial state:
   * currentIndex is invalid.
   * The view has called setCurrentIndex(0,0).
   *
   * The item at index 0,0 is current, but not selected.
   */
  QCOMPARE( selectionModel.currentIndex().row(), 0 );

  /*
   * Now disallow changing current row
   */
  selectionModel.setChangeCurrentRowAllowed(false);

  /*
   * State: index 0,0 is current (but not selected)
   *
   * The user wants to navigate to another row
   *
   * We tell the view to got to index 1,0
   * The selection model does not allow this row change
   * Index 0,0 is still current
   *
   * Then the view calls select(1,0,ClearAndSelect).
   * The selection must not change.
   */
  setViewCurrentIndexToRowAndColumn(view, 1, 0);
  QCOMPARE( selectionModel.currentIndex().row(), 0 );
  selectRowAndColumn(view, 1, 0);
  QVERIFY( !indexByRowAndColumnIsSelected(selectionModel, 1, 0) );

  /*
   * Now allow changing row
   *
   * Nothing more happens for the moment
   */
  selectionModel.setChangeCurrentRowAllowed(true);
  QCOMPARE( selectionModel.currentIndex().row(), 0 );
  QVERIFY( !indexByRowAndColumnIsSelected(selectionModel, 1, 0) );

  /*
   * State: index 0,0 is current
   *
   * The user wants to navigate to another row
   *
   * The selection model sets the correct index to 1,0
   *
   * The item at index 1,0 is current and selected
   * The previously selected item at index 0,0 is not selected anymore
   */
  setViewCurrentIndexToRowAndColumn(view, 1, 0);
  QCOMPARE( selectionModel.currentIndex().row(), 1 );
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 1, 0) );
  QVERIFY( !indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
}

void ItemSelectionModelWidgetsTest::treeViewTest()
{
  ReadOnlyTableModel model;
  model.setTable({{1,"A"},{2,"B"},{3,"C"}});

  QTreeView view;
  view.setModel(&model);

  ItemSelectionModel selectionModel(&model);
  view.setSelectionModel(&selectionModel);

  /*
   * We use the default selection mode and behaviour of the view
   * Because the test can depend on those,
   * check that they are the expected ones.
   */
  QVERIFY( view.selectionBehavior() == QAbstractItemView::SelectRows );
  QVERIFY( view.selectionMode() == QAbstractItemView::SingleSelection );

  /*
   * Wait until the view is visible and has called setCurrentIndex(0,0)
   */
  view.show();
  QApplication::setActiveWindow(&view);
  QVERIFY( waitForWindowActive(view) );

  /*
   * Initial state:
   * currentIndex is invalid.
   * The view has called setCurrentIndex(0,0).
   *
   * The item at index 0,0 is current.
   */
  QCOMPARE( selectionModel.currentIndex().row(), 0 );

  /*
   * Now disallow changing current row
   */
  selectionModel.setChangeCurrentRowAllowed(false);

  /*
   * State: index 0,0 is current (but not selected)
   *
   * The user wants to navigate to another row
   *
   * We tell the view to got to index 1,0
   * The selection model does not allow this row change
   * Index 0,0 is still current
   *
   * Then the view calls select(1,0,ClearAndSelect).
   * The selection must not change.
   */
  setViewCurrentIndexToRowAndColumn(view, 1, 0);
  QCOMPARE( selectionModel.currentIndex().row(), 0 );
  selectRowAndColumn(view, 1, 0);
  QVERIFY( !indexByRowAndColumnIsSelected(selectionModel, 1, 0) );

  /*
   * Now allow changing row
   *
   * Nothing more happens for the moment
   */
  selectionModel.setChangeCurrentRowAllowed(true);
  QCOMPARE( selectionModel.currentIndex().row(), 0 );
  QVERIFY( !indexByRowAndColumnIsSelected(selectionModel, 1, 0) );

  /*
   * State: index 0,0 is current
   *
   * The user wants to navigate to another row
   *
   * The selection model sets the correct index to 1,0
   *
   * The item at index 1,0 is current and selected
   * The previously selected item at index 0,0 is not selected anymore
   */
  setViewCurrentIndexToRowAndColumn(view, 1, 0);
  QCOMPARE( selectionModel.currentIndex().row(), 1 );
  QVERIFY( indexByRowAndColumnIsSelected(selectionModel, 1, 0) );
  QVERIFY( !indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
}

void ItemSelectionModelWidgetsTest::modelReset()
{
  ReadOnlyTableModel model;

  QTableView view;
  view.setModel(&model);

  ItemSelectionModel selectionModel(&model);
  view.setSelectionModel(&selectionModel);

  /*
   * Wait until the view is visible and has called setCurrentIndex(0,0)
   */
  view.show();
  QApplication::setActiveWindow(&view);
  QVERIFY( waitForWindowActive(view) );

  model.setTable({{1,"A"},{2,"B"},{3,"C"}});

  QVERIFY( !selectionModel.currentIndex().isValid() );

  selectionModel.setCurrentIndexToFirstRowAfterReset(true);

  model.setTable({{1,"A"},{2,"B"},{3,"C"}});

  QCOMPARE( selectionModel.currentIndex().row(), 0 );
}

void ItemSelectionModelWidgetsTest::currentIndexIsSetWithCorrectRowAndColumn()
{
  ReadOnlyTableModel model;
  model.setTable({{1,"A"},{2,"B"},{3,"C"}});

  QTableView view;
  view.setModel(&model);

  ItemSelectionModel selectionModel(&model);
  view.setSelectionModel(&selectionModel);

  /*
   * Wait until the view is visible and has called setCurrentIndex(0,0)
   */
  view.show();
  QApplication::setActiveWindow(&view);
  QVERIFY( waitForWindowActive(view) );

  setViewCurrentIndexToRowAndColumn(view, 0, 0);
  QCOMPARE( view.currentIndex().row(), 0 );
  QCOMPARE( view.currentIndex().column(), 0 );

  setViewCurrentIndexToRowAndColumn(view, 0, 1);
  QCOMPARE( view.currentIndex().row(), 0 );
  QCOMPARE( view.currentIndex().column(), 1 );

  setViewCurrentIndexToRowAndColumn(view, 1, 0);
  QCOMPARE( view.currentIndex().row(), 1 );
  QCOMPARE( view.currentIndex().column(), 0 );

  setViewCurrentIndexToRowAndColumn(view, 1, 1);
  QCOMPARE( view.currentIndex().row(), 1 );
  QCOMPARE( view.currentIndex().column(), 1 );
}

QTEST_MAIN(ItemSelectionModelWidgetsTest)
