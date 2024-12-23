// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef APPEND_ROW_TABLE_MODEL_H
#define APPEND_ROW_TABLE_MODEL_H

#include "Mdt/ItemModel/TestLib/TableModelCommonBase.h"


class AppendRowTableModel : public Mdt::ItemModel::TestLib::TableModelCommonBase
{
 Q_OBJECT

 public:

  explicit AppendRowTableModel(QObject *parent = nullptr)
   : TableModelCommonBase(parent)
  {
  }

 private:

  bool doSupportsAppendRow() const noexcept override
  {
    return true;
  }

  void doAppendRow() noexcept override
  {
    appendRecordToTable( Record() );
  }
};

#endif // #ifndef APPEND_ROW_TABLE_MODEL_H
