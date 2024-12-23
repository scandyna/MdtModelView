// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef DEVICE_LIST_TABLE_H
#define DEVICE_LIST_TABLE_H

#include <vector>
#include <string>

struct DeviceListRecord
{
  int id = 0;
  std::string description;
};

using DeviceListTable = std::vector<DeviceListRecord>;

#endif // #ifndef DEVICE_LIST_TABLE_H
