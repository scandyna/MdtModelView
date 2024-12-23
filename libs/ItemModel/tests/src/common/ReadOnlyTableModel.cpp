// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ReadOnlyTableModel.h"
#include <QString>
#include <cassert>

QVariant ReadOnlyTableModel::displayRoleData(const QModelIndex & index) const noexcept
{
  assert( indexIsValidAndInRange(index) );

  assert( index.row() >= 0 );
  const size_t row = static_cast<size_t>( index.row() );

  const auto column = static_cast<Column>( index.column() );
  switch(column){
    case Column::Value:
      return mTable[row].value;
    case Column::Name:
      return QString::fromStdString(mTable[row].name);
  }

  return QVariant();
}
