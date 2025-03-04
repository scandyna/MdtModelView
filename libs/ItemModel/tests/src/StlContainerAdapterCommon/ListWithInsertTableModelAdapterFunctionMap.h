// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef LIST_WITH_INSERT_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
#define LIST_WITH_INSERT_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H

#include "ListWithInsert.h"

struct ListWithInsertTableModelAdapterFunctionMap
{
  using size_type = ListWithInsert::size_type;
  using const_reference = const Item &;
  using difference_type = ListWithInsert::difference_type;
  using const_iterator = ListWithInsert::const_iterator;

  static
  size_type size(const ListWithInsert & list) noexcept
  {
    return list.getSizeCustom();
  }

  static
  const_reference atIndex(const ListWithInsert & list, size_type index) noexcept
  {
    return list.itemAt(index);
  }

  static
  void insert(ListWithInsert & list, const_iterator pos, size_type count, const_reference item)
  {
    list.insert(pos, count, item);
  }

  static
  const_iterator cbegin(const ListWithInsert & list) noexcept
  {
    return list.cbegin();
  }
};

#endif // #ifndef LIST_WITH_INSERT_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
