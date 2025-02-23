// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MUTABLE_LIST_H
#define MUTABLE_LIST_H

#include "Item.h"
#include <vector>

struct MutableList
{
  using size_type = std::vector<Item>::size_type;

  size_type getSizeCustom() const noexcept
  {
    return 25;
  }

  const Item & itemAt(size_type index) const noexcept
  {
  }

  Item & mutableItemAt(size_type index) noexcept
  {
  }
};

#endif // #ifndef MUTABLE_LIST_H
