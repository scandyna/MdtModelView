// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef LIST_WITH_INSERT_AND_LIMIT_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
#define LIST_WITH_INSERT_AND_LIMIT_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H

#include "ListWithInsertAndLimit.h"

struct ListWithInsertAndLimitTableModelAdapterFunctionMap
{
  using size_type = ListWithInsertAndLimit::size_type;
  using const_reference = const Item &;
  using difference_type = ListWithInsertAndLimit::difference_type;
  using const_iterator = ListWithInsertAndLimit::const_iterator;

  static
  size_type size(const ListWithInsertAndLimit & list) noexcept
  {
    return list.getSizeCustom();
  }

  static
  size_type maxSize(const ListWithInsertAndLimit & list) noexcept
  {
    return list.maximumElementCount();
  }

  static
  const_reference atIndex(const ListWithInsertAndLimit & list, size_type index) noexcept
  {
    return list.itemAt(index);
  }

  static
  void insert(ListWithInsertAndLimit & list, const_iterator pos, size_type count, const_reference item)
  {
    list.insert(pos, count, item);
  }

  static
  const_iterator cbegin(const ListWithInsertAndLimit & list) noexcept
  {
    return list.cbegin();
  }
};

#endif // #ifndef LIST_WITH_INSERT_AND_LIMIT_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
