// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef APPEND_RECORD_TABLE_MODEL_H
#define APPEND_RECORD_TABLE_MODEL_H

#include "Mdt/ItemModel/TestLib/TableModelCommonBase.h"

class AppendRecordTableModel : public Mdt::ItemModel::TestLib::TableModelCommonBase
{
 Q_OBJECT

 public:

  AppendRecordTableModel(QObject *parent = nullptr)
   : TableModelCommonBase(parent)
  {
  }

  void appendRecord(const Record & record)
  {
    beginAppendRow();
    appendRecordToTable(record);
    endAppendRow();
  }
};

#endif // #ifndef APPEND_RECORD_TABLE_MODEL_H
