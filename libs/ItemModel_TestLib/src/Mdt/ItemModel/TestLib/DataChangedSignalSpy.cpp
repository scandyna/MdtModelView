// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "DataChangedSignalSpy.h"

namespace Mdt{ namespace ItemModel{ namespace TestLib{

DataChangedSignalRecord::DataChangedSignalRecord(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles) noexcept
 : mTopLeftIndex(topLeft),
   mBottomRightIndex(bottomRight),
   mRoles(roles)
{
}

DataChangedSignalSpy::DataChangedSignalSpy(QAbstractItemModel & model) noexcept
 : QObject()
{
  connect(&model, &QAbstractItemModel::dataChanged, this, &DataChangedSignalSpy::addRecord);
}

void DataChangedSignalSpy::addRecord(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles) noexcept
{
  mList.emplace_back(topLeft, bottomRight, roles);
}

}}} // namespace Mdt{ namespace ItemModel{ namespace TestLib{
