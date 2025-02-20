// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef DEFAULT_CONSTRUCTIBLE_ONLY_LIST_H
#define DEFAULT_CONSTRUCTIBLE_ONLY_LIST_H

#include "Item.h"
#include <vector>

class DefaultConstructibleOnlyList
{
 public:

  using size_type = std::vector<Item>::size_type;

  DefaultConstructibleOnlyList() noexcept = default;
  DefaultConstructibleOnlyList(const DefaultConstructibleOnlyList &) = delete;
  DefaultConstructibleOnlyList(DefaultConstructibleOnlyList &&) = delete;

  size_type getSizeCustom() const noexcept
  {
    return mList.size();
  }

  const Item & itemAt(size_type index) const noexcept
  {
    return mList.at(index);
  }

 private:

  std::vector<Item> mList;
};

#endif // #ifndef DEFAULT_CONSTRUCTIBLE_ONLY_LIST_H
