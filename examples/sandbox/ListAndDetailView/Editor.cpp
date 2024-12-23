// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "Editor.h"
#include <cassert>

#include <QDebug>

Editor::Editor(std::shared_ptr<DeviceLibrary> deviceLibrary, QObject *parent)
 : QObject(parent),
   mDeviceLibrary(deviceLibrary)
{
  assert(mDeviceLibrary.get() != nullptr);
}

void Editor::setCurrentDevice(const QVariant & deviceId)
{
  qDebug() << "Editor, request id " << deviceId;

  /// \todo assert we can change device

  mDevice = mDeviceLibrary->fetchById( deviceId.toInt() );

  emit currentDeviceChanged( makeViewData() );

  if( mDevice.has_value() ){
    mCurrentState = EditorState::visualizing();
  }else{
    mCurrentState = EditorState::visualizingNone();
  }
  emit editorStateChanged(mCurrentState);
}

void Editor::setEditingStarted() noexcept
{
  if( mDevice.has_value() ){
    mCurrentState = EditorState::editing();
  }else{
    mCurrentState = EditorState::editingNew();
  }

  emit editorStateChanged(mCurrentState);
}

void Editor::cancelChanges() noexcept
{
  emit deviceDataChanged( makeViewData() );

  mCurrentState = EditorState::visualizing();
  emit editorStateChanged(mCurrentState);
}

void Editor::saveChanges(const DeviceDetailViewData & data)
{
  /// \todo validation ?

  Device device;

  device.id = data.id.toInt();
  device.description = data.description.toStdString();
  device.detail = data.detail.toStdString();

  const int id = mDeviceLibrary->saveDevice(device);

//   setCurrentDevice(id);

  mDevice = mDeviceLibrary->fetchById(id);

  emit deviceDataChanged( makeViewData() );

  mCurrentState = EditorState::visualizing();
  emit editorStateChanged(mCurrentState);
}

void Editor::deleteDevice()
{
  assert( mCurrentState.canDeleteDevice() );
  assert( mDevice.has_value() );

  mDeviceLibrary->deleteDevice(mDevice->id);

  // setCurrentDevice() will be called by UI
}

DeviceDetailViewData Editor::makeViewData() noexcept
{
  DeviceDetailViewData data;

  if( mDevice.has_value() ){
    data.id = QString::number(mDevice->id);
    data.description = QString::fromStdString(mDevice->description);
    data.detail = QString::fromStdString(mDevice->detail);
  }

  return data;
}
