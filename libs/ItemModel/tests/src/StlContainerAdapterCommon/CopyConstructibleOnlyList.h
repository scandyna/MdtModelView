// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef COPY_CONSTRUCTIBLE_ONLY_LIST_H
#define COPY_CONSTRUCTIBLE_ONLY_LIST_H

#include "Item.h"
#include <vector>

class CopyConstructibleOnlyList
{
 public:

  using size_type = std::vector<Item>::size_type;

  CopyConstructibleOnlyList() = delete;
  CopyConstructibleOnlyList(const CopyConstructibleOnlyList &) = default;
  CopyConstructibleOnlyList(CopyConstructibleOnlyList &&) = delete;

  size_type getSizeCustom() const noexcept
  {
    return mList.size();
  }

  const Item & itemAt(size_type index) const noexcept
  {
    return mList.at(index);
  }

  static
  CopyConstructibleOnlyList fromItemList(const std::vector<Item> & itemList) noexcept
  {
  }

 private:

  std::vector<Item> mList;
};

#endif // #ifndef COPY_CONSTRUCTIBLE_ONLY_LIST_H
