// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef READ_ONLY_TABLE_MODEL_H
#define READ_ONLY_TABLE_MODEL_H

#include "Mdt/ItemModel/AbstractTableModel.h"
#include "Mdt/Numeric/BasicConversion.h"
#include <QVariant>
#include <vector>
#include <string>

class ReadOnlyTableModel : public Mdt::ItemModel::AbstractTableModel
{
  Q_OBJECT

 public:

  struct Record
  {
    int value;
    std::string name;
  };

  using Table = std::vector<Record>;

  enum class Column
  {
    Value = 0,
    Name = 1
  };

  ReadOnlyTableModel(QObject *parent = nullptr)
   : AbstractTableModel(parent)
  {
  }

  void setTable(const Table & table)
  {
    beginResetModel();
    mTable = table;
    endResetModel();
  }

  void clear()
  {
    beginResetModel();
    mTable.clear();
    endResetModel();
  }

 private:

  int rowCountWithoutParentIndex() const noexcept override
  {
    return Mdt::Numeric::int_from_size_t( mTable.size() );
  }

  int columnCountWithoutParentIndex() const noexcept override
  {
    return 2;
  }

  QVariant displayRoleData(const QModelIndex & index) const noexcept override;

  Table mTable;
};

#endif // #ifndef READ_ONLY_TABLE_MODEL_H
