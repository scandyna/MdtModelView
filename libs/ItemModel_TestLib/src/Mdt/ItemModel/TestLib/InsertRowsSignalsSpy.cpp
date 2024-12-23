// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "InsertRowsSignalsSpy.h"


namespace Mdt{ namespace ItemModel{ namespace TestLib{

RowsAboutToBeInsertedSignalRecord::RowsAboutToBeInsertedSignalRecord(const QModelIndex & parent, int first, int last) noexcept
 : mParentIndex(parent),
   mFirst(first),
   mLast(last)
{
}


RowsInsertedSignalRecord::RowsInsertedSignalRecord(const QModelIndex & parent, int first, int last) noexcept
 : mParentIndex(parent),
   mFirst(first),
   mLast(last)
{
}

bool operator==(const RowsAboutToBeInsertedSignalRecord & rowsAboutToBeInsertedRecord, const RowsInsertedSignalRecord & rowsInsertedRecord) noexcept
{
  return rowsAboutToBeInsertedRecord.parentIndex() == rowsInsertedRecord.parentIndex()
      && rowsAboutToBeInsertedRecord.first() == rowsInsertedRecord.first()
      && rowsAboutToBeInsertedRecord.last() == rowsInsertedRecord.last();
}


InsertRowsSignalsSpy::InsertRowsSignalsSpy(QAbstractItemModel & model) noexcept
 : QObject()
{
  connect(&model, &QAbstractItemModel::rowsAboutToBeInserted, this, &InsertRowsSignalsSpy::addRowsAboutToBeInsertedRecord);
  connect(&model, &QAbstractItemModel::rowsInserted, this, &InsertRowsSignalsSpy::addRowsInsertedRecord);
}

void InsertRowsSignalsSpy::addRowsAboutToBeInsertedRecord(const QModelIndex & parent, int first, int last) noexcept
{
  mRowsAboutToBeInsertedList.emplace_back(parent, first, last);
}

void InsertRowsSignalsSpy::addRowsInsertedRecord(const QModelIndex & parent, int first, int last) noexcept
{
  mRowsInsertedList.emplace_back(parent, first, last);
}

}}} // namespace Mdt{ namespace ItemModel{ namespace TestLib{
