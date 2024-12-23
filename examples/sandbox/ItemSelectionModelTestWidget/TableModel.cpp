// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "TableModel.h"
#include "Mdt/ItemModel/StlHelpers.h"
#include <QString>
#include <cassert>

using namespace Mdt::ItemModel;


TableModel::TableModel(QObject *parent)
 : AbstractTableModel(parent)
{
}

void TableModel::setTable(const Table & table)
{
  beginResetModel();
  mTable = table;
  endResetModel();
}

int TableModel::rowCountWithoutParentIndex() const noexcept
{
  return static_cast<int>( mTable.size() );
}

int TableModel::columnCountWithoutParentIndex() const noexcept
{
  return 2;
}

QVariant TableModel::displayRoleData(const QModelIndex & index) const noexcept
{
  assert( indexIsValidAndInRange(index) );

  const auto row = static_cast<size_t>( index.row() );
  const auto column = static_cast<Column>( index.column() );

  switch(column){
    case Column::Id:
      return mTable[row].id;
    case Column::Name:
      return QString::fromStdString(mTable[row].name);
  }

  return QVariant();
}

bool TableModel::doSupportsInsertRows() const noexcept
{
  return true;
}

void TableModel::doInsertRows(int row, int count) noexcept
{
  assert( rowAndCountIsValidForInsertRows(row, count) );

  insertToStlContainer( mTable, row, count, Record() );
}

bool TableModel::doSupportsRemoveRows() const noexcept
{
  return true;
}

void TableModel::doRemoveRows(int row, int count) noexcept
{
  assert( rowAndCountIsValidForRemoveRows(row, count) );

  removeFromStlContainer(mTable, row, count);
}
