// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "RowRangeList.h"
#include "RowRangeListAlgorithm.h"

namespace Mdt{ namespace ItemModel{

void RowRangeList::addRange(const RowRange & range) noexcept
{
  if( mList.empty() ){
    mList.push_back(range);
    return;
  }

  const auto insertPoint = findPotentialInsertionPoint(mList.begin(), mList.end(), range);
  const auto firstToMergePoint = findFirstElementToMerge(mList.begin(), insertPoint, mList.end(), range);

  if( firstToMergePoint == mList.end() ){
    mList.insert(insertPoint, range);
  }else{
    const auto end = mergeRangesUntil(firstToMergePoint, mList.end(), range);
    mList.erase( end, mList.end() );
  }

  assert( isSorted(mList) );
  assert( elementsAreNotMergeable(mList) );
}

}} // namespace Mdt{ namespace ItemModel{
