// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "EditableTableModel.h"
#include <QString>

void EditableTableModel::setRecord(int row, const Record & record)
{
  assert( rowIndexIsInRange(row) );

  const size_t sRow = static_cast<size_t>(row);

  mTable[sRow] = record;

  emitRowDataChanged(row);
}

QVariant EditableTableModel::displayRoleData(const QModelIndex & index) const noexcept
{
  assert( indexIsValidAndInRange(index) );

  assert( index.row() >= 0 );
  const size_t row = static_cast<size_t>( index.row() );

  const auto column = static_cast<Column>( index.column() );
  switch(column){
    case Column::Id:
      return mTable[row].id;
    case Column::Name:
      return QString::fromStdString(mTable[row].name);
  }

  return QVariant();
}

bool EditableTableModel::setEditRoleData(const QModelIndex & index, const QVariant & value) noexcept
{
  assert( indexIsValidAndInRange(index) );

  assert( index.row() >= 0 );
  const size_t row = static_cast<size_t>( index.row() );

  const auto column = static_cast<Column>( index.column() );
  switch(column){
    case Column::Id:
      mTable[row].id = value.toInt();
      return true;
    case Column::Name:
      mTable[row].name = value.toString().toStdString();
      return true;
  }

  return false;
}
