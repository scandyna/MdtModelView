// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef REMOVE_ROWS_TABLE_MODEL_H
#define REMOVE_ROWS_TABLE_MODEL_H

#include "Mdt/ItemModel/TestLib/TableModelCommonBase.h"


class RemoveRowsTableModel : public Mdt::ItemModel::TestLib::TableModelCommonBase
{
  Q_OBJECT

  public:

  RemoveRowsTableModel(QObject *parent = nullptr)
  : TableModelCommonBase(parent)
  {
  }

  private:

  bool doSupportsRemoveRows() const noexcept override
  {
    return true;
  }

  void doRemoveRows(int row, int count) noexcept override
  {
    assert( rowAndCountIsValidForRemoveRows(row, count) );

    removeRowsFromTable(row, count);
  }
};

#endif // #ifndef REMOVE_ROWS_TABLE_MODEL_H
