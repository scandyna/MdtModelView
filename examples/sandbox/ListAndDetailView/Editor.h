// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef EDITOR_H
#define EDITOR_H

#include "Device.h"
#include "DeviceLibrary.h"
#include <QObject>
#include <QString>
#include <QLatin1String>
#include <QVariant>
#include <optional>
#include <memory>

struct DeviceDetailViewData
{
  QString id;
  QString description;
  QString detail;
};

class EditorState
{
  enum class State
  {
    Visualizing,
    VisualizingNone,
    Editing,
    EditingNew
  };

 public:

  EditorState() noexcept = default;

  QString debugName() const noexcept
  {
    switch(mState){
      case State::Visualizing:
        return QLatin1String("Visualizing");
      case State::VisualizingNone:
        return QLatin1String("VisualizingNone");
      case State::Editing:
        return QLatin1String("Editing");
      case State::EditingNew:
        return QLatin1String("EditingNew");
    }

    return QLatin1String("??");
  }

  bool canChangeCurrentDevice() const noexcept
  {
    switch(mState){
      case State::Visualizing:
      case State::VisualizingNone:
        return true;
      case State::Editing:
      case State::EditingNew:
        return false;
    }

    return false;
  }

  bool haveSomethingToSave() const noexcept
  {
    switch(mState){
      case State::Editing:
      case State::EditingNew:
        return true;
      case State::Visualizing:
      case State::VisualizingNone:
        return false;
    }

    return false;
  }

  bool canDeleteDevice() const noexcept
  {
    switch(mState){
      case State::Visualizing:
      case State::Editing:
        return true;
      case State::VisualizingNone:
      case State::EditingNew:
        return false;
    }

    return false;
  }

  static
  EditorState visualizing() noexcept
  {
    return EditorState(State::Visualizing);
  }

  static
  EditorState visualizingNone() noexcept
  {
    return EditorState(State::VisualizingNone);
  }

  static
  EditorState editing() noexcept
  {
    return EditorState(State::Editing);
  }

  static
  EditorState editingNew() noexcept
  {
    return EditorState(State::EditingNew);
  }

 private:

  EditorState(State state) noexcept
   : mState(state)
  {
  }

  State mState = State::VisualizingNone;
};

/** \todo Should editor hold a pointer to the list model ??
 *
 * - Getting the id would be done here for setCurrentDevice()
 * - when editing, updating the list could be done here
 *
 * \note Not dependent of sort/filter on the view (!)
 */
class Editor : public QObject
{
  Q_OBJECT

 public:

  explicit
  Editor(std::shared_ptr<DeviceLibrary> deviceLibrary, QObject *parent = nullptr);

  EditorState currentState() const noexcept
  {
    return mCurrentState;
  }

  void setCurrentDevice(const QVariant & deviceId);

 public slots:

  void setEditingStarted() noexcept;
  void cancelChanges() noexcept;
  void saveChanges(const DeviceDetailViewData & data);
  void deleteDevice();

 signals:

  void currentDeviceChanged(const DeviceDetailViewData & data);
  void deviceDataChanged(const DeviceDetailViewData & data);
//   void currentDeviceSaved(const DeviceDetailViewData & data);
  void editorStateChanged(const EditorState & state);

 private:

  DeviceDetailViewData makeViewData() noexcept;

  std::optional<Device> mDevice;
  std::shared_ptr<DeviceLibrary> mDeviceLibrary;
  EditorState mCurrentState;
};

#endif // #ifndef EDITOR_H
