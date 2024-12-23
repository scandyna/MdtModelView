// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef PREPEND_ROW_TABLE_MODEL_H
#define PREPEND_ROW_TABLE_MODEL_H

#include "Mdt/ItemModel/TestLib/TableModelCommonBase.h"
#include <QVariant>
#include <vector>
#include <string>

class PrependRowTableModel : public Mdt::ItemModel::TestLib::TableModelCommonBase
{
  Q_OBJECT

 public:

  explicit PrependRowTableModel(QObject *parent = nullptr)
   : TableModelCommonBase(parent)
  {
  }

 private:

  bool doSupportsPrependRow() const noexcept override
  {
    return true;
  }

  void doPrependRow() noexcept override
  {
    prependRecordToTable( Record() );
  }
};

#endif // #ifndef PREPEND_ROW_TABLE_MODEL_H
