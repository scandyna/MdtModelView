// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef DEVICE_LIST_TABLE_MODEL_H
#define DEVICE_LIST_TABLE_MODEL_H

#include "DeviceListTable.h"
#include "Mdt/ItemModel/AbstractTableModel.h"
#include <QObject>
#include <QVariant>
#include <string>


class DeviceListTableModel : public Mdt::ItemModel::AbstractTableModel
{
  Q_OBJECT

 public:

  enum class Column
  {
    Id,
    Description
  };

  static
  constexpr
  int idColumn()
  {
    return static_cast<int>(Column::Id);
  }

  DeviceListTableModel(QObject *parent = nullptr);

  void setRecord(int row, const DeviceListRecord & record) noexcept;

  void setTable(const DeviceListTable & table);

 private:

  int rowCountWithoutParentIndex() const noexcept override;
  int columnCountWithoutParentIndex() const noexcept override;
  QVariant horizontalHeaderDisplayRoleData(int column) const noexcept override;
  QVariant displayRoleData(const QModelIndex & index) const noexcept override;
  bool setEditRoleData(const QModelIndex & index, const QVariant & value) noexcept override;
  bool doSupportsInsertRows() const noexcept override;
  void doInsertRows(int row, int count) noexcept override;
  bool doSupportsRemoveRows() const noexcept override;
  void doRemoveRows(int row, int count) noexcept override;

  DeviceListTable mTable;
};

#endif // #ifndef DEVICE_LIST_TABLE_MODEL_H
