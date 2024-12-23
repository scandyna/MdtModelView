// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef DEVICE_LIBRARY_H
#define DEVICE_LIBRARY_H

#include "Device.h"
#include "DeviceListTable.h"
#include <vector>
#include <optional>

/*! \brief Device library
 *
 * This is simplistic repository.
 *
 * Here it has dependencies that should not exist.
 * This is for simplicity.
 *
 * As an example, it is depending DeviceListTable
 * to fetch the list of devices, without the details.
 *
 * In a real world, some fetcher class should be made
 * to avoid such coupling and provide a flexible way to fetch items.
 */
class DeviceLibrary
{
 public:

  DeviceLibrary();

  DeviceListTable fetchAll();

  std::optional<Device> fetchById(int id);

  /// \todo repository should provide create und update - Editor is responsible to choose
  int saveDevice(const Device & device);
//   void updateDevice(const Device & device);

  void deleteDevice(int id);

 private:

  int getNewId() const;

  std::vector<Device> mList;
};

#endif // #ifndef DEVICE_LIBRARY_H
