// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef INSERT_ROWS_TABLE_MODEL_H
#define INSERT_ROWS_TABLE_MODEL_H

#include "Mdt/ItemModel/TestLib/TableModelCommonBase.h"


class InsertRowsTableModel : public Mdt::ItemModel::TestLib::TableModelCommonBase
{
  Q_OBJECT

 public:

  InsertRowsTableModel(QObject *parent = nullptr)
   : TableModelCommonBase(parent)
  {
  }

 private:

  bool doSupportsInsertRows() const noexcept override
  {
    return true;
  }

  void doInsertRows(int row, int count) noexcept override
  {
    insertRecordToTable( row, count, Record() );
  }
};

#endif // #ifndef INSERT_ROWS_TABLE_MODEL_H
