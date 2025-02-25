// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "MutableListRawData.h"

MutableListRawData MutableListRawData::fromItemList(const std::vector<Item> & itemList) noexcept
{
  MutableListRawData list;
  list.mList = itemList;

  return list;
}
