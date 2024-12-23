// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef DEFAULT_HEADER_TABLE_MODEL_H
#define DEFAULT_HEADER_TABLE_MODEL_H

#include "Mdt/ItemModel/AbstractTableModel.h"

/*
 * A table model that defines no header
 */
class DefaultHeaderTableModel : public Mdt::ItemModel::AbstractTableModel
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
};

#endif // #ifndef DEFAULT_HEADER_TABLE_MODEL_H
