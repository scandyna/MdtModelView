// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef DEFAULT_CONSTRUCTIBLE_ONLY_LIST_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
#define DEFAULT_CONSTRUCTIBLE_ONLY_LIST_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H

#include "DefaultConstructibleOnlyList.h"

struct DefaultConstructibleOnlyListTableModelAdapterFunctionMap
{
  using size_type = DefaultConstructibleOnlyList::size_type;
  using const_reference = const Item &;

  static
  size_type size(const DefaultConstructibleOnlyList & list) noexcept
  {
    return list.getSizeCustom();
  }

  static
  const_reference atIndex(const DefaultConstructibleOnlyList & list, size_type index) noexcept
  {
    return list.itemAt(index);
  }
};

#endif // #ifndef COPY_CONSTRUCTIBLE_ONLY_LIST_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
