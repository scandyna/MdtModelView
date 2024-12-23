// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef CUSTOM_HEADER_TABLE_MODEL_H
#define CUSTOM_HEADER_TABLE_MODEL_H

#include "Mdt/ItemModel/AbstractTableModel.h"
#include <QVariant>

/*
 * A table model with custom header
 */
class CustomHeaderTableModel : public Mdt::ItemModel::AbstractTableModel
{
 private:

  int rowCountWithoutParentIndex() const noexcept override
  {
    return 2;
  }

  int columnCountWithoutParentIndex() const noexcept override
  {
    return 3;
  }

  QVariant displayRoleData(const QModelIndex &) const noexcept override
  {
    return QVariant();
  }

  QVariant horizontalHeaderDisplayRoleData(int column) const noexcept override;

  QVariant verticalHeaderDisplayRoleData(int row) const noexcept override;
};

#endif // #ifndef CUSTOM_HEADER_TABLE_MODEL_H
