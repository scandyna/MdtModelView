// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef DEVICE_H
#define DEVICE_H

#include <string>

struct Device
{
  int id;
  std::string description;
  std::string detail;
};

#endif // #ifndef DEVICE_H
