// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "RowSelection.h"
#include "RowSelectionHelpers.h"
#include <QItemSelectionRange>

namespace Mdt{ namespace ItemModel{

RowSelection RowSelection::fromItemSelection(const QItemSelection & itemSelection) noexcept
{
  RowSelection rowSelection;

  for(const QItemSelectionRange & itemRange : itemSelection){
    rowSelection.mRowRangeList.addRange( rowRangeFromItemSelectionRange(itemRange) );
  }

  return rowSelection;
}

}} // namespace Mdt{ namespace ItemModel{
