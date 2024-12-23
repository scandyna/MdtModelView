// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef EDITABLE_TABLE_MODEL_H
#define EDITABLE_TABLE_MODEL_H

#include "Mdt/ItemModel/AbstractTableModel.h"
#include "Mdt/Numeric/BasicConversion.h"
#include <QVariant>
#include <vector>
#include <string>

class EditableTableModel : public Mdt::ItemModel::AbstractTableModel
{
  Q_OBJECT

 public:

  struct Record
  {
    int id;
    std::string name;
  };

  using Table = std::vector<Record>;

  enum class Column
  {
    Id = 0,
    Name = 1
  };

  EditableTableModel(QObject *parent = nullptr)
   : AbstractTableModel(parent)
  {
  }

  Qt::ItemFlags flags(const QModelIndex & index) const override
  {
    if( !indexIsValidAndInRange(index) ){
      return AbstractTableModel::flags(index);
    }
    // Here we assume that all columns are editable
    return AbstractTableModel::flags(index) | Qt::ItemIsEditable;
  }

  void setRecord(int row, const Record & record);

  void setTable(const Table & table)
  {
    mTable = table;
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

  bool setEditRoleData(const QModelIndex & index, const QVariant & value) noexcept override;

  Table mTable;
};

#endif // #ifndef EDITABLE_TABLE_MODEL_H
