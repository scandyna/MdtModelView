// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef READ_ONLY_LIST_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
#define READ_ONLY_LIST_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H

#include "ReadOnlyList.h"

struct ReadOnlyListTableModelAdapterFunctionMap
{
  using size_type = ReadOnlyList::size_type;
  using const_reference = const Item &;

  static
  size_type size(const ReadOnlyList & list) noexcept
  {
    return list.getSizeCustom();
  }

  static
  const_reference atIndex(const ReadOnlyList & list, size_type index) noexcept
  {
    return list.itemAt(index);
  }
};

#endif // #ifndef READ_ONLY_LIST_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
