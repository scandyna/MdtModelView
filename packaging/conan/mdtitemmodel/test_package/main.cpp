// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2024-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#include <Mdt/ItemModel/AbstractTableModel.h>
#include <Mdt/ItemModel/Helpers.h>
#include <QVariant>
#include <QDebug>
#include <array>
#include <vector>
#include <cassert>

class ReadOnlyTableModel : public Mdt::ItemModel::AbstractTableModel
{
  Q_OBJECT

 public:

  using Record = std::array<QVariant, 2>;
  using Table = std::vector<Record>;

  explicit
  ReadOnlyTableModel(const Table & table, QObject *parent = nullptr)
   : AbstractTableModel(parent),
     mTable(table)
  {
  }

 private:

  int rowCountWithoutParentIndex() const noexcept override
  {
    return mTable.size();
  }

  int columnCountWithoutParentIndex() const noexcept override
  {
    return 2;
  }

  QVariant horizontalHeaderDisplayRoleData(int column) const noexcept override
  {
    assert( columnIndexIsInRange(column) );
    switch(column){
      case 0:
        return "value";
      case 1:
        return "name";
    }
    return QVariant();
  }

  QVariant displayRoleData(const QModelIndex & index) const noexcept override
  {
    assert( indexIsValidAndInRange(index) );
    return mTable[index.row()][index.column()];
  }

  Table mTable;
};

int main()
{
  ReadOnlyTableModel model({{1,"A"},{2,"B"}});

  qDebug() << "table contains " << model.rowCount() << " rows";
  qDebug() << " data(0,0): " << Mdt::ItemModel::getModelData(model, 0, 0);

  return 0;
}

#include "main.moc"
