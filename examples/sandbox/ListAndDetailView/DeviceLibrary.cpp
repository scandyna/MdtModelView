// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "DeviceLibrary.h"
#include <algorithm>
#include <cassert>

using namespace std::string_literals;


template<typename It>
It deviceLibraryFindIteratorById(It begin, It end, int id)
{
  const auto pred = [id](const Device & device){
    return device.id == id;
  };

  return std::find_if(begin, end, pred);
}

DeviceLibrary::DeviceLibrary()
{
  mList.emplace_back( Device{1, "A"s, "Detail A"s} );
  mList.emplace_back( Device{2, "B"s, "Detail B"s} );
//   mList.emplace_back( Device{3, "C"s, "Detail C"s} );
}

DeviceListTable DeviceLibrary::fetchAll()
{
  DeviceListTable table;

  for(const Device & device : mList){
    DeviceListRecord record;
    record.id = device.id;
    record.description = device.description;
    table.push_back(record);
  }

  return table;
}

std::optional<Device> DeviceLibrary::fetchById(int id)
{
  const auto it = deviceLibraryFindIteratorById(mList.cbegin(), mList.cend(), id);
  if( it == mList.cend() ){
    return {};
  }

  return *it;
}

int DeviceLibrary::saveDevice(const Device & device)
{
  const auto it = deviceLibraryFindIteratorById(mList.begin(), mList.end(), device.id);
  if( it == mList.end() ){
    const int id = getNewId();
    mList.push_back(device);
    mList.back().id = id;
    return id;
  }else{
    *it = device;
    return device.id;
  }
}

void DeviceLibrary::deleteDevice(int id)
{
  const auto it = deviceLibraryFindIteratorById(mList.cbegin(), mList.cend(), id);
  assert( it != mList.cend() );

  mList.erase(it);
}

// void DeviceLibrary::updateDevice(const Device & device)
// {
//   const auto it = deviceLibraryFindIteratorById(mList.begin(), mList.end(), device.id);
//   assert( it != mList.end() );
//
//   it->description = device.description;
//   it->detail = device.detail;
// }

int DeviceLibrary::getNewId() const
{
  if( mList.empty() ){
    return 1;
  }

  return mList.back().id + 1;
}
