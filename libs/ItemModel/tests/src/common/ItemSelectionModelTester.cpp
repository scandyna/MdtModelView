// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ItemSelectionModelTester.h"
#include <cassert>

using namespace Mdt::ItemModel;


ItemSelectionModelTester::ItemSelectionModelTester(ItemSelectionModel *selectionModel)
 : mSelectionModel(selectionModel)
{
  assert( !mSelectionModel.isNull() );

  connect(mSelectionModel, &ItemSelectionModel::currentRowChanged, this, &ItemSelectionModelTester::appendCurrentRowChanged);
}

void ItemSelectionModelTester::clearCounters() noexcept
{
  mCurrentRowChangedCount = 0;
}

void ItemSelectionModelTester::appendCurrentRowChanged(const QModelIndex &/*current*/, const QModelIndex &/*previous*/)
{
  ++mCurrentRowChangedCount;
}
