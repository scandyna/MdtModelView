// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef LIST_WITH_INSERT_AND_LIMIT_H
#define LIST_WITH_INSERT_AND_LIMIT_H

#include "Item.h"
#include <vector>

class ListWithInsertAndLimit
{
 public:

  using size_type = std::vector<Item>::size_type;
  using difference_type = std::vector<Item>::difference_type;
  using const_iterator = std::vector<Item>::const_iterator;

  size_type getSizeCustom() const noexcept
  {
    return mList.size();
  }

  void setMaximumElementCount(size_type max) noexcept
  {
    mMaximumElementCount = max;
  }

  size_type maximumElementCount() const noexcept
  {
    return mMaximumElementCount;
  }

  const Item & itemAt(size_type index) const noexcept
  {
    return mList.at(index);
  }

  void insert(const_iterator pos, size_type count, const Item & item)
  {
    mList.insert(pos, count, item);
  }

  const_iterator cbegin() const noexcept
  {
    return mList.cbegin();
  }

  const_iterator cend() const noexcept
  {
    return mList.cend();
  }

 private:

  size_type mMaximumElementCount = 500;
  std::vector<Item> mList;
};

#endif // #ifndef LIST_WITH_INSERT_AND_LIMIT_H
