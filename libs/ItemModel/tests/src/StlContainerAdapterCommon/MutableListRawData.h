// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MUTABLE_LIST_RAW_DATA_H
#define MUTABLE_LIST_RAW_DATA_H

#include "Item.h"
#include <vector>

class MutableListRawData
{
 public:

  using size_type = std::vector<Item>::size_type;

  size_type getSizeCustom() const noexcept
  {
    return mList.size();
  }

  const Item & itemAt(size_type index) const noexcept
  {
    return mList.at(index);
  }

  Item & mutableItemAt(size_type index) noexcept
  {
    return mList.at(index);
  }

  static
  MutableListRawData fromItemList(const std::vector<Item> & itemList) noexcept;

 private:

  std::vector<Item> mList;
};

#endif // #ifndef MUTABLE_LIST_RAW_DATA_H
