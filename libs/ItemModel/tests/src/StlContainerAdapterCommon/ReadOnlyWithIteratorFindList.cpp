// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ReadOnlyWithIteratorFindList.h"

ReadOnlyWithIteratorFindList ReadOnlyWithIteratorFindList::fromItemList(const std::vector<Item> & itemList) noexcept
{
  ReadOnlyWithIteratorFindList list;
  list.mList = itemList;

  return list;
}
