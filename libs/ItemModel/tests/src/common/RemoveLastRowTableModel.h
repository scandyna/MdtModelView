// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef REMOVE_LAST_ROW_TABLE_MODEL_H
#define REMOVE_LAST_ROW_TABLE_MODEL_H

#include "Mdt/ItemModel/TestLib/TableModelCommonBase.h"

class RemoveLastRowTableModel : public Mdt::ItemModel::TestLib::TableModelCommonBase
{
 Q_OBJECT

 public:

  RemoveLastRowTableModel(QObject *parent = nullptr)
   : TableModelCommonBase(parent)
  {
  }

 private:

  bool doSupportsRemoveLastRow() const noexcept override
  {
    return true;
  }

  void doRemoveLastRow() noexcept override
  {
    removeLastRowFromTable();
  }
};

#endif // #ifndef REMOVE_LAST_ROW_TABLE_MODEL_H
