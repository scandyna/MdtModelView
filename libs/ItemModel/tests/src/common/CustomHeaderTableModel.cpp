// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "CustomHeaderTableModel.h"
#include <QLatin1String>
#include <cassert>

QVariant CustomHeaderTableModel::horizontalHeaderDisplayRoleData(int column) const noexcept
{
  assert( columnIndexIsInRange(column) );

  switch(column){
    case 0:
      return QLatin1String("A");
    case 1:
      return QLatin1String("B");
    case 2:
      return QLatin1String("C");
  }

  return QVariant();
}

QVariant CustomHeaderTableModel::verticalHeaderDisplayRoleData(int row) const noexcept
{
  assert( rowIndexIsInRange(row) );

  return row + 10;
}
