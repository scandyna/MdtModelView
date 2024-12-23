// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "AbstractTableModel.h"
#include <cassert>

namespace Mdt{ namespace ItemModel{

AbstractTableModel::AbstractTableModel(QObject *parent) noexcept
 : QAbstractTableModel(parent)
{
}

int AbstractTableModel::rowCount(const QModelIndex & parent) const
{
  if( parent.isValid() ){
    return 0;
  }

  return rowCountWithoutParentIndex();
}

int AbstractTableModel::columnCount(const QModelIndex & parent) const
{
  if( parent.isValid() ){
    return 0;
  }

  return columnCountWithoutParentIndex();
}

QVariant AbstractTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(role != Qt::DisplayRole){
    return QVariant();
  }

  if(orientation == Qt::Horizontal){
    if( !columnIndexIsInRange(section) ){
      return QVariant();
    }
    return horizontalHeaderDisplayRoleData(section);
  }else{
    if( !rowIndexIsInRange(section) ){
      return QVariant();
    }
    return verticalHeaderDisplayRoleData(section);
  }

  return QVariant();
}

bool AbstractTableModel::indexIsValidAndInRange(const QModelIndex & index) const noexcept
{
  if( !index.isValid() ){
    return false;
  }

  if( !rowIndexIsInRange( index.row() ) ){
    return false;
  }

  if( !columnIndexIsInRange( index.column() ) ){
    return false;
  }

  return true;
}

QVariant AbstractTableModel::data(const QModelIndex & index, int role) const
{
  if( !indexIsValidAndInRange(index) ){
    return QVariant();
  }

  switch(role){
    case Qt::DisplayRole:
      return displayRoleData(index);
    case Qt::EditRole:
      return editRoleData(index);
  }

  return otherRoleData(index, role);
}

bool AbstractTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
  if( !indexIsValidAndInRange(index) ){
    return false;
  }

  if( role == Qt::EditRole ){
    if( setEditRoleData(index, value) ){
      emit dataChanged(index, index);
      return true;
    }
  }else if( role == Qt::DisplayRole ){
    if( setDisplayRoleData(index, value) ){
      emit dataChanged(index, index);
      return true;
    }
  }else{
    if( setOtherRoleData(index, value, role) ){
      emit dataChanged(index, index);
      return true;
    }
  }

  return false;
}

bool AbstractTableModel::rowAndCountIsValidForInsertRows(int row, int count) const noexcept
{
  if( row < 0 ){
    return false;
  }
  if( row > rowCountWithoutParentIndex() ){
    return false;
  }
  if( count < 1 ){
    return false;
  }

  return true;
}

bool AbstractTableModel::rowAndCountRepresentsPrependRow(int row, int count) const noexcept
{
  assert( rowAndCountIsValidForInsertRows(row, count) );

  if( count != 1 ){
    return false;
  }
  if( row != 0 ){
    return false;
  }

  return true;
}

bool AbstractTableModel::rowAndCountRepresentsAppendRow(int row, int count) const noexcept
{
  assert( rowAndCountIsValidForInsertRows(row, count) );

  if( count != 1 ){
    return false;
  }
  if( row != rowCountWithoutParentIndex() ){
    return false;
  }

  return true;
}

bool AbstractTableModel::insertRows(int row, int count, const QModelIndex & parent)
{
  if( parent.isValid() ){
    return false;
  }
  if( !rowAndCountIsValidForInsertRows(row, count) ){
    return false;
  }

  enum class InsertMethod
  {
    InsertRows,
    PrependRow,
    AppendRow
  };

  InsertMethod insertMethod;
  if( supportsInsertRows() ){
    insertMethod = InsertMethod::InsertRows;
  }else if( supportsPrependRow() && rowAndCountRepresentsPrependRow(row, count) ){
    insertMethod = InsertMethod::PrependRow;
  }else if( supportsAppendRow() && rowAndCountRepresentsAppendRow(row, count) ){
    insertMethod = InsertMethod::AppendRow;
  }else{
    return false;
  }

  const int first = row;
  const int last = first + count-1;
  assert( last >= first );

  beginInsertRows( QModelIndex(), first, last );

  switch(insertMethod){
    case InsertMethod::InsertRows:
      doInsertRows(row, count);
      break;
    case InsertMethod::PrependRow:
      doPrependRow();
      break;
    case InsertMethod::AppendRow:
      doAppendRow();
      break;
  }

  endInsertRows();

  return true;
}

bool AbstractTableModel::rowAndCountIsValidForRemoveRows(int row, int count) const noexcept
{
  if(row < 0){
    return false;
  }
  if(count < 1){
    return false;
  }
  if( (row + count) > rowCountWithoutParentIndex() ){
    return false;
  }

  return true;
}

bool AbstractTableModel::rowAndCountRepresentsRemoveFirstRow(int row, int count) const noexcept
{
  assert( rowAndCountIsValidForRemoveRows(row, count) );

  if( count != 1 ){
    return false;
  }
  if( row != 0 ){
    return false;
  }

  return true;
}

bool AbstractTableModel::rowAndCountRepresentsRemoveLastRow(int row, int count) const noexcept
{
  assert( rowAndCountIsValidForRemoveRows(row, count) );

  if( count != 1 ){
    return false;
  }
  if( row != ( rowCountWithoutParentIndex() - 1) ){
    return false;
  }

  return true;
}

bool AbstractTableModel::removeRows(int row, int count, const QModelIndex & parent)
{
  if( parent.isValid() ){
    return false;
  }
  if( !rowAndCountIsValidForRemoveRows(row, count) ){
    return false;
  }

  enum class RemoveMethod
  {
    RemoveRows,
    RemoveFirstRow,
    RemoveLastRow
  };

  RemoveMethod removeMethod;
  if( supportsRemoveRows() ){
    removeMethod = RemoveMethod::RemoveRows;
  }else if( supportsRemoveFirstRow() && rowAndCountRepresentsRemoveFirstRow(row, count) ){
    removeMethod = RemoveMethod::RemoveFirstRow;
  }else if( supportsRemoveLastRow() && rowAndCountRepresentsRemoveLastRow(row, count) ){
    removeMethod = RemoveMethod::RemoveLastRow;
  }else{
    return false;
  }

  const int first = row;
  const int last = first + count-1;
  assert( last >= first );

  beginRemoveRows(QModelIndex(), first, last);

  switch(removeMethod){
    case RemoveMethod::RemoveRows:
      doRemoveRows(row, count);
      break;
    case RemoveMethod::RemoveFirstRow:
      doRemoveFirstRow();
      break;
    case RemoveMethod::RemoveLastRow:
      doRemoveLastRow();
      break;
  }

  endRemoveRows();

  return true;
}

QVariant AbstractTableModel::horizontalHeaderDisplayRoleData(int column) const noexcept
{
  assert( columnIndexIsInRange(column) );

  return QAbstractTableModel::headerData(column, Qt::Horizontal, Qt::DisplayRole);
}

QVariant AbstractTableModel::verticalHeaderDisplayRoleData(int row) const noexcept
{
  assert( rowIndexIsInRange(row) );

  return QAbstractTableModel::headerData(row, Qt::Vertical, Qt::DisplayRole);
}

QVariant AbstractTableModel::editRoleData(const QModelIndex & index) const
{
  assert( indexIsValidAndInRange(index) );

  return displayRoleData(index);
}

QVariant AbstractTableModel::otherRoleData(const QModelIndex&, int) const
{
  return QVariant();
}

bool AbstractTableModel::setEditRoleData(const QModelIndex &, const QVariant &) noexcept
{
  return false;
}

bool AbstractTableModel::setDisplayRoleData(const QModelIndex & index, const QVariant & value) noexcept
{
  assert( indexIsValidAndInRange(index) );

  return setEditRoleData(index, value);
}

bool AbstractTableModel::setOtherRoleData(const QModelIndex&, const QVariant&, int)
{
  return false;
}

void AbstractTableModel::emitRowDataChanged(int row, const QVector<int> & roles) noexcept
{
  assert( columnCount() >= 1 );
  assert( rowIndexIsInRange(row) );

  QModelIndex topLeft = index(row, 0);
  QModelIndex bottomRight = index( row, columnCount()-1 );

  emit dataChanged(topLeft, bottomRight, roles);
}

void AbstractTableModel::beginAppendRow()
{
  const int first = rowCount();
  const int last = first;

  beginInsertRows( QModelIndex(), first, last );
}

void AbstractTableModel::endAppendRow()
{
  endInsertRows();
}

void AbstractTableModel::doInsertRows(int, int) noexcept
{
}

void AbstractTableModel::doRemoveRows(int, int) noexcept
{
}

}} // namespace Mdt{ namespace ItemModel{
