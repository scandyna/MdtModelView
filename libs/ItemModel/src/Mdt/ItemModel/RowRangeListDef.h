// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_ROW_RANGE_LIST_DEF_H
#define MDT_ITEM_MODEL_ROW_RANGE_LIST_DEF_H

#include "Mdt/ItemModel/RowRange.h"
#include <QtGlobal>
#include <vector>

#ifdef Q_CC_MSVC
  #pragma warning( push )
  #pragma warning( disable : 4251 )
#endif

namespace Mdt{ namespace ItemModel{

  /*! \internal
   */
  using RowRangeListContainer = std::vector<RowRange>;

  /*! \internal
   */
  using RowRangeListIterator = RowRangeListContainer::iterator;

  /*! \internal
   */
  using RowRangeListConstIterator = RowRangeListContainer::const_iterator;

  /*! \internal
   */
  using RowRangeListConstReverseIterator = RowRangeListContainer::const_reverse_iterator;

}} // namespace Mdt{ namespace ItemModel{

#ifdef Q_CC_MSVC
  #pragma warning( pop )
#endif

#endif // #ifndef MDT_ITEM_MODEL_ROW_RANGE_LIST_DEF_H
