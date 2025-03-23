// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef READ_ONLY_WITH_ITERATOR_FIND_LIST_H
#define READ_ONLY_WITH_ITERATOR_FIND_LIST_H

#include "Item.h"
#include <vector>

class ReadOnlyWithIteratorFindList
{
 public:

  using size_type = std::vector<Item>::size_type;
  using difference_type = std::vector<Item>::difference_type;
  using const_iterator = std::vector<Item>::const_iterator;

  size_type getSizeCustom() const noexcept
  {
    return mList.size();
  }

  const Item & itemAt(size_type index) const noexcept
  {
    return mList.at(index);
  }

  // const_iterator findItemWithId(int id) const noexcept
  // {
  // }

  const_iterator cbegin() const noexcept
  {
    return mList.cbegin();
  }

  const_iterator cend() const noexcept
  {
    return mList.cend();
  }

  static
  ReadOnlyWithIteratorFindList fromItemList(const std::vector<Item> & itemList) noexcept;

 private:

  std::vector<Item> mList;
};

#endif // #ifndef READ_ONLY_WITH_ITERATOR_FIND_LIST_H
