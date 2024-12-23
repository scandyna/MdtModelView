// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "DeviceListTableModel.h"
#include "Mdt/ItemModel/StlHelpers.h"
#include <QString>
#include <cassert>

using namespace Mdt::ItemModel;


DeviceListTableModel::DeviceListTableModel(QObject *parent)
 : AbstractTableModel(parent)
{
}

void DeviceListTableModel::setRecord(int row, const DeviceListRecord & record) noexcept
{
  assert( rowIndexIsInRange(row) );

  const auto sRow = static_cast<size_t>(row);
  mTable[sRow] = record;

  emitRowDataChanged(row);
}

void DeviceListTableModel::setTable(const DeviceListTable & table)
{
  beginResetModel();
  mTable = table;
  endResetModel();
}

int DeviceListTableModel::rowCountWithoutParentIndex() const noexcept
{
  return static_cast<int>( mTable.size() );
}

int DeviceListTableModel::columnCountWithoutParentIndex() const noexcept
{
  return 2;
}

QVariant DeviceListTableModel::horizontalHeaderDisplayRoleData(int column) const noexcept
{
  assert( columnIndexIsInRange(column) );

  const auto columnEnum = static_cast<Column>(column);

  switch(columnEnum){
    case Column::Id:
      return tr("Id");
    case Column::Description:
      return tr("Description");
  }

  return QVariant();
}

QVariant DeviceListTableModel::displayRoleData(const QModelIndex & index) const noexcept
{
  assert( indexIsValidAndInRange(index) );

  const auto row = static_cast<size_t>( index.row() );
  const auto column = static_cast<Column>( index.column() );

  switch(column){
    case Column::Id:
      return mTable[row].id;
    case Column::Description:
      return QString::fromStdString(mTable[row].description);
  }

  return QVariant();
}

bool DeviceListTableModel::setEditRoleData(const QModelIndex & index, const QVariant & value) noexcept
{
  assert( indexIsValidAndInRange(index) );

  const auto row = static_cast<size_t>( index.row() );
  const auto column = static_cast<Column>( index.column() );

  switch(column){
    case Column::Id:
      mTable[row].id = value.toInt();
      return true;
    case Column::Description:
      mTable[row].description = value.toString().toStdString();
      return true;
  };

  return false;
}

bool DeviceListTableModel::doSupportsInsertRows() const noexcept
{
  return true;
}

void DeviceListTableModel::doInsertRows(int row, int count) noexcept
{
  assert( rowAndCountIsValidForInsertRows(row, count) );

  insertToStlContainer( mTable, row, count, DeviceListRecord() );
}

bool DeviceListTableModel::doSupportsRemoveRows() const noexcept
{
  return true;
}

void DeviceListTableModel::doRemoveRows(int row, int count) noexcept
{
  assert( rowAndCountIsValidForRemoveRows(row, count) );

  removeFromStlContainer(mTable, row, count);
}
