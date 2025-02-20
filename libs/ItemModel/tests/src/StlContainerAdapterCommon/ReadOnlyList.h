// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef READ_ONLY_LIST_H
#define READ_ONLY_LIST_H

#include "Item.h"
#include <vector>

/*! \brief Read only list
 *
 * The only STL container conformity
 * is providing size_type .
 */
class ReadOnlyList
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

  static
  ReadOnlyList fromItemList(const std::vector<Item> & itemList) noexcept;

 private:

  std::vector<Item> mList;
};

#endif // #ifndef READ_ONLY_LIST_H
