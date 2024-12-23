// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef ITEM_SELECTION_MODEL_TEST_WIDGET_H
#define ITEM_SELECTION_MODEL_TEST_WIDGET_H

#include "TableModel.h"
#include "Mdt/ItemModel/ItemSelectionModel.h"
#include "ui_ItemSelectionModelTestWidget.h"
#include <QWidget>
#include <QItemSelectionModel>
#include <QSortFilterProxyModel>
#include <QModelIndex>
#include <QTimer>
#include <QString>

class ItemSelectionModelTestWidget : public QWidget
{
  Q_OBJECT

 public:

  explicit ItemSelectionModelTestWidget(QWidget *parent = nullptr);

 private slots:

  void setQtSelectionModelIf(bool checked) noexcept;
  void setMdtSelectionModelIf(bool checked) noexcept;

  void useSortFilterProfyModelIf(int checkState) noexcept;

  void populateModel() noexcept;

  void prependItem() noexcept;
  void appendItem() noexcept;
  void removeSelectedItems() noexcept;

  void displayTableViewCurrentIndex(const QModelIndex & current) noexcept;
  void displayTableViewCurrentChangedEmitted() noexcept;
  void resetDisplayTableViewCurrentChangedEmitted() noexcept;
  void displayListViewCurrentIndex(const QModelIndex & current) noexcept;
  void displayTreeViewCurrentIndex(const QModelIndex & current) noexcept;

  void displayAllCurrentIndex() noexcept;

 private:

  void connectItemSelectionModelsSignals() noexcept;
  void disconnectItemSelectionModelsSignals() noexcept;

  void applyFilter() noexcept;

  static
  QString formatIndex(const QModelIndex & index) noexcept;

  Ui::ItemSelectionModelTestWidget mUi;
  TableModel mTableModel;
  TableModel mFakeTableModel;
  QSortFilterProxyModel mTableViewSortFilterModel;
  QSortFilterProxyModel mListViewSortFilterModel;
  QSortFilterProxyModel mTreeViewSortFilterModel;
  QItemSelectionModel mTableViewQtSelectionModel;
  QItemSelectionModel mListViewQtSelectionModel;
  QItemSelectionModel mTreeViewQtSelectionModel;
  Mdt::ItemModel::ItemSelectionModel mTableViewMdtSelectionModel;
  Mdt::ItemModel::ItemSelectionModel mListViewMdtSelectionModel;
  Mdt::ItemModel::ItemSelectionModel mTreeViewMdtSelectionModel;
  QTimer mTableViewCurrentChangedDisplayResetTimer;
};

#endif // #ifndef ITEM_SELECTION_MODEL_TEST_WIDGET_H
