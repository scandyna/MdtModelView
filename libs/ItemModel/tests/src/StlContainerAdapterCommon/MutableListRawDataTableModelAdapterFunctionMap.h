// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MUTABLE_LIST_RAW_DATA_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
#define MUTABLE_LIST_RAW_DATA_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H

#include "MutableListRawData.h"

struct MutableListRawDataTableModelAdapterFunctionMap
{
  using size_type = MutableListRawData::size_type;
  using const_reference = const Item &;
  using reference = Item &;

  static
  size_type size(const MutableListRawData & list) noexcept
  {
    return list.getSizeCustom();
  }

  static
  const_reference atIndex(const MutableListRawData & list, size_type index) noexcept
  {
    return list.itemAt(index);
  }

  static
  reference atIndexMutable(MutableListRawData & list, size_type index) noexcept
  {
    return list.mutableItemAt(index);
  }
};

#endif // #ifndef MUTABLE_LIST_RAW_DATA_TABLE_MODEL_ADAPTER_FUNCTION_MAP_H
