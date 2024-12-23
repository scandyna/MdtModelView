// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "Helpers.h"
#include "RowSelection.h"
#include <QItemSelectionRange>
#include <cassert>


namespace Mdt{ namespace ItemModel{

bool modelRowAndColumnAreInRange(const QAbstractItemModel & model, int row, int column) noexcept
{
  if(row < 0){
    return false;
  }
  if(column < 0){
    return false;
  }

  return ( row < model.rowCount() ) && ( column < model.columnCount() );
}

QVariant getModelData(const QAbstractItemModel &model, int row, int column, Qt::ItemDataRole role)
{
  assert( row >= 0 );
  assert( row < model.rowCount() );
  assert( column >= 0 );
  assert( column < model.columnCount() );

  const auto index = model.index(row, column);
  assert( index.isValid() );

  return model.data(index, role);
}

bool setModelData(QAbstractItemModel & model, int row, int column, const QVariant & value, Qt::ItemDataRole role)
{
  assert( row >= 0 );
  assert( row < model.rowCount() );
  assert( column >= 0 );
  assert( column < model.columnCount() );

  const auto index = model.index(row, column);
  assert( index.isValid() );

  return model.setData(index, value, role);
}

bool prependRowToModel(QAbstractItemModel & model)
{
  return model.insertRows(0, 1);
}

bool appendRowToModel(QAbstractItemModel & model)
{
  return model.insertRows(model.rowCount(), 1);
}

bool removeFirstRowFromModel(QAbstractItemModel & model)
{
  return model.removeRow(0);
}

bool removeLastRowFromModel(QAbstractItemModel & model)
{
  const int row = model.rowCount() - 1;

  if(row < 0){
    return false;
  }

  return model.removeRow(row);
}

bool removeSelectedRows(QItemSelectionModel *selectionModel)
{
  assert( selectionModel != nullptr );
  assert( selectionModel->model() != nullptr );

  auto *model = selectionModel->model();
  const QItemSelection itemSelection = selectionModel->selection();
  const auto rowSelection = RowSelection::fromItemSelection(itemSelection);

  auto rFirst = rowSelection.crbegin();
  const auto rLast = rowSelection.crend();

  while(rFirst != rLast){
    const RowRange & range = *rFirst;
    if( !model->removeRows( range.firstRow(), range.rowCount() ) ){
      return false;
    }
    ++rFirst;
  }

  return true;
}

bool itemSelectionRangeIsSingleRow(const QItemSelectionRange & range) noexcept
{
  return range.top() == range.bottom();
}

bool itemSelectionIsSingleRow(const QItemSelection & selection) noexcept
{
  if( selection.isEmpty() ){
    return false;
  }

  if( !itemSelectionRangeIsSingleRow( selection.at(0) ) ){
    return false;
  }

  int firstRow = selection.at(0).top();
  for(int i = 1; i < selection.size(); ++i){
    if( !itemSelectionRangeIsSingleRow( selection.at(i) ) ){
      return false;
    }
    if( selection.at(i).top() != firstRow ){
      return false;
    }
  }

  return true;
}

void updateRowInSingleRowItemSelection(int row, QItemSelection & selection) noexcept
{
  assert( row >= 0 );
  assert( itemSelectionIsSingleRow(selection) );

  for(QItemSelectionRange & range : selection){
    auto *model = range.model();
    assert(model != nullptr);
    assert( row < model->rowCount() );
    QModelIndex topLeft = model->index( row, range.left() );
    QModelIndex bottomRight = model->index( row, range.right() );
    QItemSelectionRange newRange(topLeft, bottomRight);
    range.swap(newRange);
  }
}

}} // namespace Mdt{ namespace ItemModel{
