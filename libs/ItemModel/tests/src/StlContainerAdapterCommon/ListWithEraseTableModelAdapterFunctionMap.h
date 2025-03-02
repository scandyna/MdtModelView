// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef LIST_WITH_ERASE_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
#define LIST_WITH_ERASE_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H

#include "ListWithErase.h"

struct ListWithEraseTableModelAdapterFunctionMap
{
  using size_type = ListWithErase::size_type;
  using const_reference = const Item &;
  using difference_type = ListWithErase::difference_type;
  using const_iterator = ListWithErase::const_iterator;

  static
  size_type size(const ListWithErase & list) noexcept
  {
    return list.getSizeCustom();
  }

  static
  const_reference atIndex(const ListWithErase & list, size_type index) noexcept
  {
    return list.itemAt(index);
  }

  static
  void erase(ListWithErase & list, const_iterator first, const_iterator last) noexcept
  {
    list.erase(first, last);
  }

  static
  const_iterator begin(const ListWithErase & list) noexcept
  {
    return list.cbegin();
  }
};

#endif // #ifndef LIST_WITH_ERASE_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
