// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef TABLE_MODEL_H
#define TABLE_MODEL_H

#include "Mdt/ItemModel/AbstractTableModel.h"
#include <QObject>
#include <QVariant>
#include <string>
#include <vector>

struct Record
{
  int id = 0;
  std::string name;
};

class TableModel : public Mdt::ItemModel::AbstractTableModel
{
  Q_OBJECT

 public:

  using Table = std::vector<Record>;

  enum class Column
  {
    Id,
    Name
  };

  TableModel(QObject *parent = nullptr);

  void setTable(const Table & table);

 private:

  int rowCountWithoutParentIndex() const noexcept override;
  int columnCountWithoutParentIndex() const noexcept override;
  QVariant displayRoleData(const QModelIndex & index) const noexcept override;
  bool doSupportsInsertRows() const noexcept override;
  void doInsertRows(int row, int count) noexcept override;
  bool doSupportsRemoveRows() const noexcept override;
  void doRemoveRows(int row, int count) noexcept override;

  Table mTable;
};

#endif // #ifndef TABLE_MODEL_H
