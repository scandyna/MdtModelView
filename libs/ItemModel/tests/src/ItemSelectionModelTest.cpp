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
#include "ItemSelectionModelTester.h"
#include "ReadOnlyTableModel.h"
#include "Mdt/ItemModel/ItemSelectionModel.h"
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QModelIndex>
#include <cassert>

using namespace Mdt::ItemModel;

void setCurrentIndexForRowAndColumn(QItemSelectionModel & selectionModel, int row, int column,
                                    QItemSelectionModel::SelectionFlags command = QItemSelectionModel::ClearAndSelect)
{
  assert(selectionModel.model() != nullptr);

  QModelIndex index = selectionModel.model()->index(row, column);
  assert( index.isValid() );

  selectionModel.setCurrentIndex(index, command);
}

QItemSelection makeSingleItemSelectionForRowAndColumn(QItemSelectionModel & selectionModel, int row, int column)
{
  assert(selectionModel.model() != nullptr);

  QModelIndex index = selectionModel.model()->index(row, column);
  assert( index.isValid() );

  return QItemSelection(index, index);
}

void selectRowAndColumn_QItemSelection(QItemSelectionModel & selectionModel, int row, int column,
                                       QItemSelectionModel::SelectionFlags command = QItemSelectionModel::ClearAndSelect)
{
  assert(selectionModel.model() != nullptr);

  QItemSelection selection = makeSingleItemSelectionForRowAndColumn(selectionModel, row, column);

  selectionModel.select(selection, command);
}

void selectRowAndColumn_QModelIndex(QItemSelectionModel & selectionModel, int row, int column,
                                    QItemSelectionModel::SelectionFlags command = QItemSelectionModel::ClearAndSelect)
{
  assert(selectionModel.model() != nullptr);

  QModelIndex index = selectionModel.model()->index(row, column);
  assert( index.isValid() );

  selectionModel.select(index, command);
}

bool indexByRowAndColumnIsSelected(QItemSelectionModel & selectionModel, int row, int column)
{
  assert(selectionModel.model() != nullptr);

  QModelIndex index = selectionModel.model()->index(row, column);
  assert( index.isValid() );

  return selectionModel.isSelected(index);
}


TEST_CASE("isMultipleItemsSelectionRange")
{
  ReadOnlyTableModel model;
  model.setTable({{1,"A"},{2,"B"},{3,"C"}});
  QItemSelectionRange range;

  SECTION("empty range")
  {
    REQUIRE( !ItemSelectionModel::isMultipleItemsSelectionRange(range) );
  }

  SECTION("0,0-0,0 (1 row and 1 column) 1 item")
  {
    QModelIndex topLeft = model.index(0, 0);
    QModelIndex topRight = model.index(0, 0);
    QItemSelectionRange range(topLeft, topRight);

    REQUIRE( !ItemSelectionModel::isMultipleItemsSelectionRange(range) );
  }

  SECTION("0,0-0,1 (1 row and 2 columns) 2 items")
  {
    QModelIndex topLeft = model.index(0, 0);
    QModelIndex topRight = model.index(0, 1);
    QItemSelectionRange range(topLeft, topRight);

    REQUIRE( ItemSelectionModel::isMultipleItemsSelectionRange(range) );
  }

  SECTION("0,0-1,0 3 items")
  {
    QModelIndex topLeft = model.index(0, 0);
    QModelIndex topRight = model.index(1, 0);
    QItemSelectionRange range(topLeft, topRight);

    REQUIRE( ItemSelectionModel::isMultipleItemsSelectionRange(range) );
  }

  SECTION("0,0-1,1 4 items")
  {
    QModelIndex topLeft = model.index(0, 0);
    QModelIndex topRight = model.index(1, 1);
    QItemSelectionRange range(topLeft, topRight);

    REQUIRE( ItemSelectionModel::isMultipleItemsSelectionRange(range) );
  }
}

TEST_CASE("isMultipleItemsSelection")
{
  ReadOnlyTableModel model;
  model.setTable({{1,"A"},{2,"B"},{3,"C"}});
  QItemSelection selection;

  SECTION("empty selection")
  {
    REQUIRE( !ItemSelectionModel::isMultipleItemsSelection(selection) );
  }

  SECTION("1 range selection")
  {
    SECTION("0,0-0,0 (1 row and 1 column) 1 item")
    {
      QModelIndex topLeft = model.index(0, 0);
      QModelIndex topRight = model.index(0, 0);
      QItemSelectionRange range(topLeft, topRight);
      selection.append(range);

      REQUIRE( !ItemSelectionModel::isMultipleItemsSelection(selection) );
    }

    SECTION("0,0-0,1 (1 row and 2 columns) 2 items")
    {
      QModelIndex topLeft = model.index(0, 0);
      QModelIndex topRight = model.index(0, 1);
      QItemSelectionRange range(topLeft, topRight);
      selection.append(range);

      REQUIRE( ItemSelectionModel::isMultipleItemsSelection(selection) );
    }

    SECTION("0,0-1,1 (2 row and 2 columns) 4 items")
    {
      QModelIndex topLeft = model.index(0, 0);
      QModelIndex bottomRight = model.index(1, 1);
      QItemSelectionRange range(topLeft, bottomRight);
      selection.append(range);

      REQUIRE( ItemSelectionModel::isMultipleItemsSelection(selection) );
    }
  }

  SECTION("2 ranges selection")
  {
    QModelIndex topLeft;
    QModelIndex bottomRight;

    SECTION("[(0,0-0,0),(0,0-0,0)] (1 row and 1 column) 1 item")
    {
      topLeft = model.index(0,0);
      bottomRight = model.index(0,0);
      selection.append( QItemSelectionRange(topLeft, bottomRight) );
      topLeft = model.index(0,0);
      bottomRight = model.index(0,0);
      selection.append( QItemSelectionRange(topLeft, bottomRight) );

      REQUIRE( !ItemSelectionModel::isMultipleItemsSelection(selection) );
    }

    SECTION("[(0,0-0,0),(0,1-0,1)] (1 row and 2 columns) 2 items")
    {
      topLeft = model.index(0,0);
      bottomRight = model.index(0,0);
      selection.append( QItemSelectionRange(topLeft, bottomRight) );
      topLeft = model.index(0,1);
      bottomRight = model.index(0,1);
      selection.append( QItemSelectionRange(topLeft, bottomRight) );

      REQUIRE( ItemSelectionModel::isMultipleItemsSelection(selection) );
    }
  }

  SECTION("3 ranges selection")
  {
    QModelIndex topLeft;
    QModelIndex bottomRight;

    SECTION("[(0,0-0,0),(0,0-0,0),(0,0-0,0)] (1 row and 1 column) 1 item")
    {
      topLeft = model.index(0,0);
      bottomRight = model.index(0,0);
      selection.append( QItemSelectionRange(topLeft, bottomRight) );
      selection.append( QItemSelectionRange(topLeft, bottomRight) );
      selection.append( QItemSelectionRange(topLeft, bottomRight) );

      REQUIRE( !ItemSelectionModel::isMultipleItemsSelection(selection) );
    }

    SECTION("[(0,0-0,0),(0,0-0,0),(0,0-0,1)] (1 row and 1 column) 1 item")
    {
      topLeft = model.index(0,0);
      bottomRight = model.index(0,0);
      selection.append( QItemSelectionRange(topLeft, bottomRight) );
      selection.append( QItemSelectionRange(topLeft, bottomRight) );
      bottomRight = model.index(0,1);
      selection.append( QItemSelectionRange(topLeft, bottomRight) );

      REQUIRE( ItemSelectionModel::isMultipleItemsSelection(selection) );
    }
  }
}

TEST_CASE("setChangeCurrentRowAllowed")
{
  ReadOnlyTableModel model;
  ItemSelectionModel selectionModel(&model);

  REQUIRE( selectionModel.changeCurrentRowIsAllowed() );

  selectionModel.setChangeCurrentRowAllowed(false);

  REQUIRE( !selectionModel.changeCurrentRowIsAllowed() );
}

TEST_CASE("canSetCurrentIndex")
{
  ReadOnlyTableModel model;
  ItemSelectionModel selectionModel(&model);
  QModelIndex index;

  model.setTable({{1,"A"},{2,"B"}});

  setCurrentIndexForRowAndColumn(selectionModel, 0, 0);
  REQUIRE( selectionModel.currentIndex().row() == 0 );
  REQUIRE( selectionModel.currentIndex().column() == 0 );

  SECTION("change current row is allowed")
  {
    selectionModel.setChangeCurrentRowAllowed(true);

    SECTION("change column works")
    {
      index = model.index(0, 1);

      REQUIRE( selectionModel.canSetCurrentIndex(index) );
    }

    SECTION("change row works")
    {
      index = model.index(1, 0);

      REQUIRE( selectionModel.canSetCurrentIndex(index) );
    }
  }

  SECTION("change current row is NOT allowed")
  {
    selectionModel.setChangeCurrentRowAllowed(false);

    SECTION("change column works")
    {
      index = model.index(0, 1);

      REQUIRE( selectionModel.canSetCurrentIndex(index) );
    }

    SECTION("change row is blocked")
    {
      index = model.index(1, 0);

      REQUIRE( !selectionModel.canSetCurrentIndex(index) );
    }
  }
}

TEST_CASE("setCurrentIndex")
{
  ReadOnlyTableModel model;
  ItemSelectionModel selectionModel(&model);

  model.setTable({{1,"A"},{2,"B"}});

  setCurrentIndexForRowAndColumn(selectionModel, 0, 0);
  REQUIRE( selectionModel.currentIndex().row() == 0 );
  REQUIRE( selectionModel.currentIndex().column() == 0 );

  SECTION("change current row is allowed")
  {
    selectionModel.setChangeCurrentRowAllowed(true);

    SECTION("change column works")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 0, 1);

      REQUIRE( selectionModel.currentIndex().row() == 0 );
      REQUIRE( selectionModel.currentIndex().column() == 1 );
    }

    SECTION("change row works")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 1, 0);

      REQUIRE( selectionModel.currentIndex().row() == 1 );
      REQUIRE( selectionModel.currentIndex().column() == 0 );
    }
  }

  SECTION("change current row is NOT allowed")
  {
    selectionModel.setChangeCurrentRowAllowed(false);

    SECTION("change column works")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 0, 1);

      REQUIRE( selectionModel.currentIndex().row() == 0 );
      REQUIRE( selectionModel.currentIndex().column() == 1 );
    }

    SECTION("change row is blocked")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 1, 0);

      REQUIRE( selectionModel.currentIndex().row() == 0 );
      REQUIRE( selectionModel.currentIndex().column() == 0 );
    }
  }
}

TEST_CASE("canSelect")
{
  ReadOnlyTableModel model;
  ItemSelectionModel selectionModel(&model);
  QItemSelection selection;

  model.setTable({{1,"A"},{2,"B"}});

  setCurrentIndexForRowAndColumn(selectionModel, 0, 0);
  REQUIRE( selectionModel.currentIndex().row() == 0 );
  REQUIRE( selectionModel.currentIndex().column() == 0 );

  SECTION("change current row is allowed")
  {
    selectionModel.setChangeCurrentRowAllowed(true);

    SECTION("select a single item in same row but different columns works")
    {
      selection = makeSingleItemSelectionForRowAndColumn(selectionModel, 0, 1);

      REQUIRE( selectionModel.canSelect(selection) );
    }

    SECTION("select a single item in another row works")
    {
      selection = makeSingleItemSelectionForRowAndColumn(selectionModel, 1, 0);

      REQUIRE( selectionModel.canSelect(selection) );
    }

    SECTION("select multiple items in different rows works")
    {
      QModelIndex topLeft = model.index(0,0);
      QModelIndex bottomRight = model.index(1,0);
      selection.append( QItemSelectionRange(topLeft, bottomRight) );

      REQUIRE( selectionModel.canSelect(selection) );
    }
  }

  SECTION("change current row is NOT allowed")
  {
    selectionModel.setChangeCurrentRowAllowed(false);

    SECTION("select a single item in same row but different columns works")
    {
      selection = makeSingleItemSelectionForRowAndColumn(selectionModel, 0, 1);

      REQUIRE( selectionModel.canSelect(selection) );
    }

    SECTION("select a single item in another row is blocked")
    {
      selection = makeSingleItemSelectionForRowAndColumn(selectionModel, 1, 0);

      REQUIRE( !selectionModel.canSelect(selection) );
    }

    SECTION("select multiple items in different rows works")
    {
      QModelIndex topLeft = model.index(0,0);
      QModelIndex bottomRight = model.index(1,0);
      selection.append( QItemSelectionRange(topLeft, bottomRight) );

      REQUIRE( selectionModel.canSelect(selection) );
    }
  }
}

TEST_CASE("setCurrentIndexToFirstRowAfterResetIsEnabled")
{
  ReadOnlyTableModel model;
  ItemSelectionModel selectionModel(&model);

  REQUIRE( !selectionModel.setCurrentIndexToFirstRowAfterResetIsEnabled() );

  selectionModel.setCurrentIndexToFirstRowAfterReset(true);
  REQUIRE( selectionModel.setCurrentIndexToFirstRowAfterResetIsEnabled() );
}

TEST_CASE("modelReset_setCurrentIndexToFirstRow_OFF")
{
  ReadOnlyTableModel model;
  ItemSelectionModel selectionModel(&model);

  model.setTable({{1,"A"},{2,"B"}});

  REQUIRE( !selectionModel.currentIndex().isValid() );
}

TEST_CASE("modelReset_setCurrentIndexToFirstRow_ON")
{
  ReadOnlyTableModel model;
  ItemSelectionModel selectionModel(&model);
  ItemSelectionModelTester tester(&selectionModel);

  selectionModel.setCurrentIndexToFirstRowAfterReset(true);

  REQUIRE( tester.currentRowChangedCount() == 0 );

  SECTION("empty model")
  {
    model.clear();

    REQUIRE( !selectionModel.currentIndex().isValid() );
    /*
     * We accept that the signals are emitted when the model was already empty before.
     * This helps keep the code simple.
     * Wait until we know that this behaviour is not acceptable before changing it.
     */
    REQUIRE( tester.currentRowChangedCount() == 1 );
  }

  SECTION("model becomes empty")
  {
    model.setTable({{1,"A"}});
    REQUIRE( selectionModel.currentIndex().row() == 0 );

    tester.clearCounters();
    model.clear();

    REQUIRE( !selectionModel.currentIndex().isValid() );
    REQUIRE( tester.currentRowChangedCount() == 1 );
  }

  SECTION("1 row model")
  {
    model.setTable({{1,"A"}});

    REQUIRE( selectionModel.currentIndex().row() == 0 );
    REQUIRE( tester.currentRowChangedCount() == 1 );
  }

  SECTION("2 rows model")
  {
    model.setTable({{1,"A"},{2,"B"}});

    REQUIRE( selectionModel.currentIndex().row() == 0 );
    REQUIRE( tester.currentRowChangedCount() == 1 );
  }
}

TEST_CASE("currentRowNotChanges_CurrentColumnChanges")
{
  ReadOnlyTableModel model;
  ItemSelectionModel selectionModel(&model);

  model.setTable({{1,"A"},{2,"B"}});

  setCurrentIndexForRowAndColumn(selectionModel, 0, 0);
  REQUIRE( selectionModel.currentIndex().row() == 0 );
  REQUIRE( selectionModel.currentIndex().column() == 0 );

  selectionModel.setChangeCurrentRowAllowed(false);

  SECTION("change current column is accepted")
  {
    setCurrentIndexForRowAndColumn(selectionModel, 0, 1);

    REQUIRE( selectionModel.currentIndex().row() == 0 );
    REQUIRE( selectionModel.currentIndex().column() == 1 );
  }

  SECTION("change current row is blocked")
  {
    setCurrentIndexForRowAndColumn(selectionModel, 1, 0);

    REQUIRE( selectionModel.currentIndex().row() == 0 );
    REQUIRE( selectionModel.currentIndex().column() == 0 );
  }
}

TEST_CASE("currentIndexIsSetWithCorrectRowAndColumn")
{
  ReadOnlyTableModel model;
  ItemSelectionModel selectionModel(&model);

  model.setTable({{1,"A"},{2,"B"}});

  setCurrentIndexForRowAndColumn(selectionModel, 0, 0);
  REQUIRE( selectionModel.currentIndex().row() == 0 );
  REQUIRE( selectionModel.currentIndex().column() == 0 );

  setCurrentIndexForRowAndColumn(selectionModel, 0, 1);
  REQUIRE( selectionModel.currentIndex().row() == 0 );
  REQUIRE( selectionModel.currentIndex().column() == 1 );

  setCurrentIndexForRowAndColumn(selectionModel, 1, 0);
  REQUIRE( selectionModel.currentIndex().row() == 1 );
  REQUIRE( selectionModel.currentIndex().column() == 0 );

  setCurrentIndexForRowAndColumn(selectionModel, 1, 1);
  REQUIRE( selectionModel.currentIndex().row() == 1 );
  REQUIRE( selectionModel.currentIndex().column() == 1 );
}


/*
 * When the user navigates with arrow keys, or selects a single item,
 * the view will call setCurrentIndex() and select().
 *
 * Here we have 2 cases (observed with QTableView, extended selection)
 *
 * A) When the user navigates with arrow keys,
 * the view will call setCurrentIndex(index, ClearAndSelect)
 * and select(selection, ClearAndSelect).
 *
 * B) When the user clicks,
 * the view will call setCurrentIndex(index, NoUpdate)
 * and select(selection, ClearAndSelect).
 *
 * If we do nothing when changing current row is not allowed,
 * setCurrentIndex() will not allow a row change,
 * so the current index will stay in current row.
 *
 * The result is the current index and the selected item are out of sync.
 * This gives the user a really bad feedback.
 *
 * To prevent this, the selection model has to filter out single item selection
 * if it is not in the current row.
 *
 *
 * For setCurrentIndex() we also have 2 cases:
 *
 * A) If we call QItemSelectionModel::setCurrentIndex(index, ClearAndSelect),
 *    it will call select(index, ClearAndSelect), which calls select(selection, ClearAndSelect).
 *
 * B) If we call QItemSelectionModel::setCurrentIndex(index, NoUpdate),
 *    select() is NOT called.
 */

TEST_CASE("selectSingleItemIsCoherentWithCurrent")
{
  ReadOnlyTableModel model;
  model.setTable({{1,"A"},{2,"B"}});

  ItemSelectionModel selectionModel(&model);

  /*
   * Initial state
   * Current index is invalid and nothing is selected
   */
  REQUIRE( !selectionModel.currentIndex().isValid() );
  REQUIRE( !indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
  REQUIRE( !indexByRowAndColumnIsSelected(selectionModel, 0, 1) );
  REQUIRE( !indexByRowAndColumnIsSelected(selectionModel, 1, 0) );
  REQUIRE( !indexByRowAndColumnIsSelected(selectionModel, 1, 1) );

  SECTION("change current row is allowed")
  {
    selectionModel.setChangeCurrentRowAllowed(true);

    SECTION("navigate to 0,0 with arrow key")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 0, 0, QItemSelectionModel::ClearAndSelect);
      selectRowAndColumn_QItemSelection(selectionModel, 0, 0, QItemSelectionModel::ClearAndSelect);
      REQUIRE( selectionModel.currentIndex().isValid() );
      REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
    }

    SECTION("click on 0,0")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 0, 0, QItemSelectionModel::NoUpdate);
      selectRowAndColumn_QItemSelection(selectionModel, 0, 0, QItemSelectionModel::ClearAndSelect);
      REQUIRE( selectionModel.currentIndex().isValid() );
      REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
    }
  }

  SECTION("change current row is NOT allowed")
  {
    selectionModel.setChangeCurrentRowAllowed(false);

    SECTION("navigate to 0,0 with arrow key")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 0, 0, QItemSelectionModel::ClearAndSelect);
      selectRowAndColumn_QItemSelection(selectionModel, 0, 0, QItemSelectionModel::ClearAndSelect);
      REQUIRE( !selectionModel.currentIndex().isValid() );
      REQUIRE( !indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
    }

    SECTION("click on 0,0")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 0, 0, QItemSelectionModel::NoUpdate);
      selectRowAndColumn_QItemSelection(selectionModel, 0, 0, QItemSelectionModel::ClearAndSelect);
      REQUIRE( !selectionModel.currentIndex().isValid() );
      REQUIRE( !indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
    }
  }

  SECTION("starting from 0,0 and change current row is allowed")
  {
    selectionModel.setChangeCurrentRowAllowed(true);

    setCurrentIndexForRowAndColumn(selectionModel, 0, 0, QItemSelectionModel::ClearAndSelect);
    selectRowAndColumn_QItemSelection(selectionModel, 0, 0, QItemSelectionModel::ClearAndSelect);
    REQUIRE( selectionModel.currentIndex().row() == 0 );
    REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 0) );

    SECTION("navigate to 0,1 with arrow key works")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 0, 1, QItemSelectionModel::ClearAndSelect);
      selectRowAndColumn_QItemSelection(selectionModel, 0, 1, QItemSelectionModel::ClearAndSelect);

      REQUIRE( selectionModel.currentIndex().row() == 0 );
      REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 1) );
      REQUIRE( !indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
    }

    SECTION("click on 0,1 works")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 0, 1, QItemSelectionModel::NoUpdate);
      selectRowAndColumn_QItemSelection(selectionModel, 0, 1, QItemSelectionModel::ClearAndSelect);

      REQUIRE( selectionModel.currentIndex().row() == 0 );
      REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 1) );
      REQUIRE( !indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
    }

    SECTION("navigate to 1,0 with arrow key works")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 1, 0, QItemSelectionModel::ClearAndSelect);
      selectRowAndColumn_QItemSelection(selectionModel, 1, 0, QItemSelectionModel::ClearAndSelect);

      REQUIRE( selectionModel.currentIndex().row() == 1 );
      REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 1, 0) );
      REQUIRE( !indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
    }

    SECTION("click on 1,0 works")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 1, 0, QItemSelectionModel::NoUpdate);
      selectRowAndColumn_QItemSelection(selectionModel, 1, 0, QItemSelectionModel::ClearAndSelect);

      REQUIRE( selectionModel.currentIndex().row() == 1 );
      REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 1, 0) );
      REQUIRE( !indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
    }
  }

  SECTION("starting from 0,0 and change current row is NOT allowed")
  {
    setCurrentIndexForRowAndColumn(selectionModel, 0, 0, QItemSelectionModel::ClearAndSelect);
    selectRowAndColumn_QItemSelection(selectionModel, 0, 0, QItemSelectionModel::ClearAndSelect);
    REQUIRE( selectionModel.currentIndex().row() == 0 );
    REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 0) );

    selectionModel.setChangeCurrentRowAllowed(false);

    SECTION("navigate to 0,1 with arrow key works")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 0, 1, QItemSelectionModel::ClearAndSelect);
      selectRowAndColumn_QItemSelection(selectionModel, 0, 1, QItemSelectionModel::ClearAndSelect);

      REQUIRE( selectionModel.currentIndex().row() == 0 );
      REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 1) );
      REQUIRE( !indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
    }

    SECTION("click on 0,1 works")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 0, 1, QItemSelectionModel::NoUpdate);
      selectRowAndColumn_QItemSelection(selectionModel, 0, 1, QItemSelectionModel::ClearAndSelect);

      REQUIRE( selectionModel.currentIndex().row() == 0 );
      REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 1) );
      REQUIRE( !indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
    }

    SECTION("navigate to 1,0 with arrow key is blocked")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 1, 0, QItemSelectionModel::ClearAndSelect);
      selectRowAndColumn_QItemSelection(selectionModel, 1, 0, QItemSelectionModel::ClearAndSelect);

      REQUIRE( selectionModel.currentIndex().row() == 0 );
      REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
      REQUIRE( !indexByRowAndColumnIsSelected(selectionModel, 1, 0) );
    }

    SECTION("click on 1,0 is blocked")
    {
      setCurrentIndexForRowAndColumn(selectionModel, 1, 0, QItemSelectionModel::NoUpdate);
      selectRowAndColumn_QItemSelection(selectionModel, 1, 0, QItemSelectionModel::ClearAndSelect);

      REQUIRE( selectionModel.currentIndex().row() == 0 );
      REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
      REQUIRE( !indexByRowAndColumnIsSelected(selectionModel, 1, 0) );
    }
  }
}

TEST_CASE("selectIsCoherentWithCurrent_QItemSelection_overload")
{
  ReadOnlyTableModel model;
  model.setTable({{1,"A"},{2,"B"}});

  ItemSelectionModel selectionModel(&model);

  setCurrentIndexForRowAndColumn(selectionModel, 0, 0);
  selectRowAndColumn_QItemSelection(selectionModel, 0, 0);
  REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 0) );

  selectionModel.setChangeCurrentRowAllowed(false);

  SECTION("select another column in the same row works")
  {
    setCurrentIndexForRowAndColumn(selectionModel, 0, 1);
    selectRowAndColumn_QItemSelection(selectionModel, 0, 1);

    REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 1) );
  }

  SECTION("select another row does NOT work")
  {
    setCurrentIndexForRowAndColumn(selectionModel, 1, 0);
    selectRowAndColumn_QItemSelection(selectionModel, 1, 0);

    REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
  }
}

TEST_CASE("selectIsCoherentWithCurrent_QModelIndex_overload")
{
  ReadOnlyTableModel model;
  model.setTable({{1,"A"},{2,"B"}});

  ItemSelectionModel selectionModel(&model);

  setCurrentIndexForRowAndColumn(selectionModel, 0, 0);
  selectRowAndColumn_QModelIndex(selectionModel, 0, 0);
  REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 0) );

  selectionModel.setChangeCurrentRowAllowed(false);

  SECTION("select another column in the same row works")
  {
    setCurrentIndexForRowAndColumn(selectionModel, 0, 1);
    selectRowAndColumn_QModelIndex(selectionModel, 0, 1);

    REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 1) );
  }

  SECTION("select another row does NOT work")
  {
    setCurrentIndexForRowAndColumn(selectionModel, 1, 0);
    selectRowAndColumn_QModelIndex(selectionModel, 1, 0);

    REQUIRE( indexByRowAndColumnIsSelected(selectionModel, 0, 0) );
  }
}
