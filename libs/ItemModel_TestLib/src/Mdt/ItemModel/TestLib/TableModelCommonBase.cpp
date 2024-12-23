// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "TableModelCommonBase.h"
#include "Mdt/ItemModel/StlHelpers.h"
#include <cassert>

using namespace Mdt::ItemModel;

namespace Mdt{ namespace ItemModel{ namespace TestLib{

void TableModelCommonBase::prependRecordToTable(const Record & record) noexcept
{
  insertRecordToTable(0, 1, record);
}

void TableModelCommonBase::insertRecordToTable(int row, int count, const Record & record) noexcept
{
  assert( rowAndCountIsValidForInsertRows(row, count) );

  const auto dRow = static_cast<Table::difference_type>(row);
  auto it = std::next(mTable.begin(), dRow);

  const auto sCount = static_cast<size_t>(count);
  mTable.insert(it, sCount, record);
}

void TableModelCommonBase::appendRecordToTable(const Record & record) noexcept
{
  mTable.push_back(record);
}

void TableModelCommonBase::removeFirstRowFromTable() noexcept
{
  assert( !mTable.empty() );

  removeFirstFromStlContainer(mTable);
}

void TableModelCommonBase::removeRowsFromTable(int row, int count) noexcept
{
  assert( rowAndCountIsValidForRemoveRows(row, count) );

  removeFromStlContainer(mTable, row, count);
}

void TableModelCommonBase::removeLastRowFromTable() noexcept
{
  assert( !mTable.empty() );

  mTable.pop_back();
}

QVariant TableModelCommonBase::displayRoleData(const QModelIndex & index) const noexcept
{
  assert( indexIsValidAndInRange(index) );

  const size_t row = Mdt::Numeric::size_t_from_int( index.row() );

  const auto column = static_cast<Column>( index.column() );
  switch(column){
    case Column::Id:
      return mTable[row].id;
    case Column::Name:
      return QString::fromStdString(mTable[row].name);
  }

  return QVariant();
}

}}} // namespace Mdt{ namespace ItemModel{ namespace TestLib{
