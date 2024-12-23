// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef LIST_AND_DETAIL_VIEW_WIDGET_H
#define LIST_AND_DETAIL_VIEW_WIDGET_H

#include "Editor.h"
#include "DeviceLibrary.h"
#include "DeviceListTableModel.h"
#include "Mdt/ItemModel/ItemSelectionModel.h"
#include "ui_ListAndDetailViewWidget.h"
#include <QItemSelectionModel>
#include <QWidget>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <memory>

class ListAndDetailViewWidget : public QWidget
{
  Q_OBJECT

 public:

  explicit
  ListAndDetailViewWidget(std::shared_ptr<DeviceLibrary> deviceLibrary, QWidget *parent = nullptr);

 public slots:

  void displayDeviceDetail(const DeviceDetailViewData & data) noexcept;
  void updateListViewCurrentDevice(const DeviceDetailViewData & data) noexcept;

  void setEditorState(const EditorState & state) noexcept;

 private slots:

  void setCurrentDeviceFromListViewRow(const QModelIndex & current, const QModelIndex & previous);

  void displayListViewCurrentChangedEvent(const QModelIndex & current) noexcept;
  void resetDisplayListViewCurrentChangedEvent() noexcept;

  void prependItem() noexcept;
  void appendItem() noexcept;
  void removeSelectedItems() noexcept;

  void applyFilter() noexcept;

  void fetchAllDevices();

  void saveDevice();
  void deleteDevice();

  void setQtSelectionModelIf(bool checked) noexcept;
  void setMdtSelectionModelIf(bool checked) noexcept;

 private:

  void connectItemSelectionModelSignals() noexcept;
  void disconnectItemSelectionModelSignals() noexcept;

  QModelIndex listViewCurrentIndex() const noexcept;

  Ui::ListAndDetailViewWidget mUi;
  Editor mEditor;
  DeviceListTableModel mListViewModel;
  QSortFilterProxyModel mListViewSortFilterModel;
  std::shared_ptr<DeviceLibrary> mDeviceLibrary;
  QTimer mResetDisplayListViewCurrentChangedEventTimer;
  std::unique_ptr<QItemSelectionModel> mListViewQtSelectionModel;
  std::unique_ptr<Mdt::ItemModel::ItemSelectionModel> mListViewMdtSelectionModel;
};

#endif // #ifndef LIST_AND_DETAIL_VIEW_WIDGET_H
