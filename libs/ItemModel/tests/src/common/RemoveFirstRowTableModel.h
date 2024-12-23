// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef REMOVE_FIRST_ROW_TABLE_MODEL_H
#define REMOVE_FIRST_ROW_TABLE_MODEL_H

#include "Mdt/ItemModel/TestLib/TableModelCommonBase.h"

class RemoveFirstRowTableModel : public Mdt::ItemModel::TestLib::TableModelCommonBase
{
 Q_OBJECT

 public:

  RemoveFirstRowTableModel(QObject *parent = nullptr)
   : TableModelCommonBase(parent)
  {
  }

 private:

  bool doSupportsRemoveFirstRow() const noexcept override
  {
    return true;
  }

  void doRemoveFirstRow() noexcept override
  {
    removeFirstRowFromTable();
  }
};

#endif // #ifndef REMOVE_FIRST_ROW_TABLE_MODEL_H