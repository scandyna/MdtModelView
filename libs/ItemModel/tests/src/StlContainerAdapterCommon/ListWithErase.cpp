// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ListWithErase.h"

ListWithErase ListWithErase::fromItemList(const std::vector<Item> & itemList) noexcept
{
  ListWithErase list;
  list.mList = itemList;

  return list;
}
