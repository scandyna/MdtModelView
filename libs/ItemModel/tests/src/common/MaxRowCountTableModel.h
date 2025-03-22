// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MAX_ROW_COUNT_TABLE_MODEL_H
#define MAX_ROW_COUNT_TABLE_MODEL_H

#include "Mdt/ItemModel/TestLib/TableModelCommonBase.h"


class MaxRowCountTableModel : public Mdt::ItemModel::TestLib::TableModelCommonBase
{
  Q_OBJECT

 public:

  MaxRowCountTableModel(QObject *parent = nullptr)
   : TableModelCommonBase(parent)
  {
  }

  void setMaxRowCount(int count)
  {
    mMaxRowCount = count;
  }

 private:

  int doMaxRowCount() const override
  {
    return mMaxRowCount;
  }

  int mMaxRowCount = 1;
};

#endif // #ifndef MAX_ROW_COUNT_TABLE_MODEL_H
