// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "RemoveRowsSignalsSpy.h"

namespace Mdt{ namespace ItemModel{ namespace TestLib{

RowsAboutToBeRemovedSignalRecord::RowsAboutToBeRemovedSignalRecord(const QModelIndex & parent, int first, int last) noexcept
 : mParentIndex(parent),
   mFirst(first),
   mLast(last)
{
}

RowsRemovedSignalRecord::RowsRemovedSignalRecord(const QModelIndex & parent, int first, int last) noexcept
 : mParentIndex(parent),
   mFirst(first),
   mLast(last)
{
}

bool operator==(const RowsAboutToBeRemovedSignalRecord & rowsAboutToBeRemovedRecord, const RowsRemovedSignalRecord & rowsRemovedRecord) noexcept
{
  return rowsAboutToBeRemovedRecord.parentIndex() == rowsRemovedRecord.parentIndex()
      && rowsAboutToBeRemovedRecord.first() == rowsRemovedRecord.first()
      && rowsAboutToBeRemovedRecord.last() == rowsRemovedRecord.last();
}

RemoveRowsSignalsSpy::RemoveRowsSignalsSpy(QAbstractItemModel & model) noexcept
 : QObject()
{
  connect(&model, &QAbstractItemModel::rowsAboutToBeRemoved, this, &RemoveRowsSignalsSpy::addRowsAboutToBeRemovedRecord);
  connect(&model, &QAbstractItemModel::rowsRemoved, this, &RemoveRowsSignalsSpy::addRowsRemovedRecord);
}

void RemoveRowsSignalsSpy::addRowsAboutToBeRemovedRecord(const QModelIndex & parent, int first, int last) noexcept
{
  mRowsAboutToBeRemovedList.emplace_back(parent, first, last);
}

void RemoveRowsSignalsSpy::addRowsRemovedRecord(const QModelIndex & parent, int first, int last) noexcept
{
  mRowsRemovedList.emplace_back(parent, first, last);
}

}}} // namespace Mdt{ namespace ItemModel{ namespace TestLib{
