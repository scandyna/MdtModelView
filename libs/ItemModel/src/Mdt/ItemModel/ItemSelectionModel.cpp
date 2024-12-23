// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ItemSelectionModel.h"
#include <QItemSelectionRange>
#include <algorithm>
#include <cassert>

namespace Mdt{ namespace ItemModel{


ItemSelectionModel::ItemSelectionModel(QAbstractItemModel* model)
 : QItemSelectionModel(model)
{
}

ItemSelectionModel::ItemSelectionModel(QAbstractItemModel* model, QObject* parent)
 : QItemSelectionModel(model, parent)
{
}

void ItemSelectionModel::setCurrentIndexToFirstRowAfterReset(bool enable) noexcept
{
  mSetCurrentIndexToFirstRowAfterResetIsEnabled = enable;
}

bool ItemSelectionModel::canSetCurrentIndex(const QModelIndex & index) noexcept
{
  if( changeCurrentRowIsAllowed() ){
    return true;
  }

  return !isRowChangeRequest(index);
}

bool ItemSelectionModel::canSelect(const QItemSelection & selection) noexcept
{
  if( changeCurrentRowIsAllowed() ){
    return true;
  }

  if( isMultipleItemsSelection(selection) ){
    return true;
  }

  return !isRowChangeRequest(selection);
}

bool ItemSelectionModel::isMultipleItemsSelectionRange(const QItemSelectionRange & range) noexcept
{
  /*
   * QItemSelectionRange::isEmpty() loops over every index.
   * Our tests shows that isValid(), which is an inline getter, works.
   * If not so, find a test that prove this wrong first.
   */
  if( !range.isValid() ){
    return false;
  }

  return range.topLeft() != range.bottomRight();
}

bool ItemSelectionModel::isMultipleItemsSelection(const QItemSelection & selection) noexcept
{
  if( selection.isEmpty() ){
    return false;
  }

  if( isMultipleItemsSelectionRange( selection.at(0) ) ){
    return true;
  }

  const auto firstMultiplePred = [](const QItemSelectionRange & range, const QItemSelectionRange & nextRange){
    if( isMultipleItemsSelectionRange(range) ){
      return true;
    }
    return range != nextRange;
  };

  const auto it = std::adjacent_find(selection.cbegin(), selection.cend(), firstMultiplePred);

  return it != selection.cend();
}

void ItemSelectionModel::setChangeCurrentRowAllowed(bool allow) noexcept
{
  mChangeCurrentRowIsAllowed = allow;
}

void ItemSelectionModel::select(const QModelIndex & index, QItemSelectionModel::SelectionFlags command)
{
  QItemSelectionModel::select(index, command);
}

/*
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
 * To prevent this, we have to filter out single item selection
 * if it is not in the current row.
 *
 * For setCurrentIndex() we also have 2 cases:
 *
 * A) If we call QItemSelectionModel::setCurrentIndex(index, ClearAndSelect),
 *    it will call select(index, ClearAndSelect), which calls select(selection, ClearAndSelect).
 *
 * B) If we call QItemSelectionModel::setCurrentIndex(index, NoUpdate),
 *    select() is NOT called.
 */
void ItemSelectionModel::select(const QItemSelection & selection, QItemSelectionModel::SelectionFlags command)
{
  if( canSelect(selection) ){
    QItemSelectionModel::select(selection, command);
  }
}

void ItemSelectionModel::setCurrentIndex(const QModelIndex & index, QItemSelectionModel::SelectionFlags command)
{
  if( canSetCurrentIndex(index) ){
    QItemSelectionModel::setCurrentIndex(index, command);
  }
}

void ItemSelectionModel::reset()
{
  QItemSelectionModel::reset();

  if( setCurrentIndexToFirstRowAfterResetIsEnabled() ){
    // QAbstractItemModel::index() does bound checking
    QModelIndex index = model()->index(0, 0);
    /*
     * If the model becomes empty, we want current changed signals to be emitted.
     * Because we have to call reset() first, current index will be invalid.
     * Calling setCurrentIndex() in this situation will not emit those signals,
     * we have to emit them here.
     */
    if( index.isValid() ){
      setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
    }else{
      emit currentChanged(index, index);
      emit currentRowChanged(index, index);
      emit currentColumnChanged(index, index);
    }
  }
}

bool ItemSelectionModel::isRowChangeRequest(int row) const noexcept
{
  return row  != currentIndex().row(); 
}

bool ItemSelectionModel::isRowChangeRequest(const QModelIndex & index) const noexcept
{
  return isRowChangeRequest( index.row() );
}

bool ItemSelectionModel::isRowChangeRequest(const QItemSelection & selection) const noexcept
{
  if( selection.isEmpty() ){
    return false;
  }
  assert( selection.count() == 1 );

  const QItemSelectionRange & range = selection.at(0);
  assert( range.isValid() );
  assert( range.top() == range.bottom() );

  return isRowChangeRequest( range.top() );
}

}} // namespace Mdt{ namespace ItemModel{
