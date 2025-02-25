// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MUTABLE_LIST_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
#define MUTABLE_LIST_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H

#include "MutableList.h"

struct MutableListTableModelAdapterFunctionMap
{
  using size_type = MutableList::size_type;
  using const_reference = const Item &;

  static
  size_type size(const MutableList & list) noexcept
  {
    return list.getSizeCustom();
  }

  static
  const_reference atIndex(const MutableList & list, size_type index) noexcept
  {
    return list.itemAt(index);
  }
};

#endif // #ifndef MUTABLE_LIST_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
