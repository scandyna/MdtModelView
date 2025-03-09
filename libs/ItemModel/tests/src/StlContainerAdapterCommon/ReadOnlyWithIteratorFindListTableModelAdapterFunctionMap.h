// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef READ_ONLY_WITH_ITERATOR_FIND_LIST_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
#define READ_ONLY_WITH_ITERATOR_FIND_LIST_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H

#include "ReadOnlyWithIteratorFindList.h"

struct ReadOnlyWithIteratorFindListTableModelAdapterFunctionMap
{
  using size_type = ReadOnlyWithIteratorFindList::size_type;
  using difference_type = ReadOnlyWithIteratorFindList::difference_type;
  using const_reference = const Item &;
  using const_iterator = ReadOnlyWithIteratorFindList::const_iterator;

  static
  size_type size(const ReadOnlyWithIteratorFindList & list) noexcept
  {
    return list.getSizeCustom();
  }

  static
  const_reference atIndex(const ReadOnlyWithIteratorFindList & list, size_type index) noexcept
  {
    return list.itemAt(index);
  }

  static
  const_iterator cbegin(const ReadOnlyWithIteratorFindList & list) noexcept
  {
    return list.cbegin();
  }
};

#endif // #ifndef READ_ONLY_WITH_ITERATOR_FIND_LIST_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
