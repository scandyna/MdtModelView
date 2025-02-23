// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MOVE_CONSTRUCTIBLE_ONLY_LIST_H
#define MOVE_CONSTRUCTIBLE_ONLY_LIST_H

#include "Item.h"
#include <vector>

class MoveConstructibleOnlyList
{
 public:

  using size_type = std::vector<Item>::size_type;

  MoveConstructibleOnlyList() = delete;
  MoveConstructibleOnlyList(const MoveConstructibleOnlyList &) = delete;
  MoveConstructibleOnlyList(MoveConstructibleOnlyList &&) noexcept = default;

  size_type getSizeCustom() const noexcept
  {
    return mList.size();
  }

  const Item & itemAt(size_type index) const noexcept
  {
    return mList.at(index);
  }

  static
  MoveConstructibleOnlyList fromItemList(const std::vector<Item> & itemList) noexcept;

 private:

  MoveConstructibleOnlyList(const std::vector<Item> & itemList)
   : mList(itemList)
  {
  }

  std::vector<Item> mList;
};

#endif // #ifndef MOVE_CONSTRUCTIBLE_ONLY_LIST_H
