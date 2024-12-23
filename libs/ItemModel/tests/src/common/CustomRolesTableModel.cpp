// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2024-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "CustomRolesTableModel.h"
#include <cassert>

QVariant CustomRolesTableModel::otherRoleData(const QModelIndex& index, int role) const
{
  assert( indexIsValidAndInRange(index) );

  if( role == Qt::ToolTipRole ){
    if( index.column() == 0 ){
      return mColumn0ToolTip;
    }
  }

  return QVariant();
}

bool CustomRolesTableModel::setOtherRoleData(const QModelIndex& index, const QVariant& value, int role)
{
  assert( indexIsValidAndInRange(index) );

  if(role != Qt::ToolTipRole){
    return false;
  }
  if(index.column() != 0){
    return false;
  }
  mColumn0ToolTip = value.toString();

  return true;
}
