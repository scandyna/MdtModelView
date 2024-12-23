// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2024-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef CUSTOM_ROLES_TABLE_MODEL_H
#define CUSTOM_ROLES_TABLE_MODEL_H

#include "Mdt/ItemModel/TestLib/TableModelCommonBase.h"
#include <QString>


class CustomRolesTableModel : public Mdt::ItemModel::TestLib::TableModelCommonBase
{
  Q_OBJECT

 public:

  CustomRolesTableModel(QObject *parent = nullptr)
   : TableModelCommonBase(parent)
  {
  }

 private:

  QVariant otherRoleData(const QModelIndex & index, int role) const override;
  bool setOtherRoleData(const QModelIndex & index, const QVariant & value, int role) override;
  QString mColumn0ToolTip = "Column 0 tool tip";
};

#endif // #ifndef CUSTOM_ROLES_TABLE_MODEL_H
