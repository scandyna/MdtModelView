// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ListAndDetailViewWidget.h"
#include "Mdt/ItemModel/Helpers.h"
#include "Mdt/ItemView/Helpers.h"
#include <QPushButton>
#include <QLineEdit>
#include <QItemSelectionModel>
#include <QVariant>
#include <QString>
#include <QLatin1String>
#include <QRegularExpression>
#include <cassert>

#include <QDebug>

using namespace Mdt::ItemModel;
using namespace Mdt::ItemView;


ListAndDetailViewWidget::ListAndDetailViewWidget(std::shared_ptr<DeviceLibrary> deviceLibrary, QWidget *parent)
 : QWidget(parent),
   mEditor(deviceLibrary),
   mDeviceLibrary(deviceLibrary)
{
  assert(mDeviceLibrary.get() != nullptr);

  mUi.setupUi(this);

  mListViewSortFilterModel.setSourceModel(&mListViewModel);
  mListViewSortFilterModel.setFilterKeyColumn(1);

  mUi.tableView->setModel(&mListViewSortFilterModel);
  mUi.tableView->setSortingEnabled(true);

  connect(mUi.id, &QLineEdit::textEdited, &mEditor, &Editor::setEditingStarted);
  connect(mUi.description, &QLineEdit::textEdited, &mEditor, &Editor::setEditingStarted);
  connect(mUi.detail, &QLineEdit::textEdited, &mEditor, &Editor::setEditingStarted);

  connect(&mEditor, &Editor::editorStateChanged, this, &ListAndDetailViewWidget::setEditorState);

  connect(&mEditor, &Editor::currentDeviceChanged, this, &ListAndDetailViewWidget::displayDeviceDetail);
  connect(&mEditor, &Editor::deviceDataChanged, this, &ListAndDetailViewWidget::displayDeviceDetail);
  connect(&mEditor, &Editor::deviceDataChanged, this, &ListAndDetailViewWidget::updateListViewCurrentDevice);

  mResetDisplayListViewCurrentChangedEventTimer.setSingleShot(true);
  mResetDisplayListViewCurrentChangedEventTimer.setInterval(1000);
  mResetDisplayListViewCurrentChangedEventTimer.setTimerType(Qt::CoarseTimer);
  connect(&mResetDisplayListViewCurrentChangedEventTimer, &QTimer::timeout, this, &ListAndDetailViewWidget::resetDisplayListViewCurrentChangedEvent);

  connect(mUi.rbQtSelectionModel, &QRadioButton::toggled, this, &ListAndDetailViewWidget::setQtSelectionModelIf);
  connect(mUi.rbMdtSelectionModel, &QRadioButton::toggled, this, &ListAndDetailViewWidget::setMdtSelectionModelIf);
  mUi.rbMdtSelectionModel->setChecked(true);

  connect(mUi.pbResetModel, &QPushButton::clicked, this, &ListAndDetailViewWidget::fetchAllDevices);
  connect(mUi.pbPrepend, &QPushButton::clicked, this, &ListAndDetailViewWidget::prependItem);
  connect(mUi.pbAppend, &QPushButton::clicked, this, &ListAndDetailViewWidget::appendItem);
  connect(mUi.pbRemove, &QPushButton::clicked, this, &ListAndDetailViewWidget::removeSelectedItems);

  mUi.filterCriteria->setText( QLatin1String("*") );
  connect(mUi.pbFilter, &QPushButton::clicked, this, &ListAndDetailViewWidget::applyFilter);

  connect(mUi.pbCancelChanges, &QPushButton::clicked, &mEditor, &Editor::cancelChanges);
  connect(mUi.pbSave, &QPushButton::clicked, this, &ListAndDetailViewWidget::saveDevice);

  connect(mUi.pbDelete, &QPushButton::clicked, this, &ListAndDetailViewWidget::deleteDevice);

  setEditorState( mEditor.currentState() );

  fetchAllDevices();
}

void ListAndDetailViewWidget::displayDeviceDetail(const DeviceDetailViewData & data) noexcept
{
  qDebug() << "displayDeviceDetail() " << data.description;

  mUi.id->setText(data.id);
  mUi.description->setText(data.description);
  mUi.detail->setText(data.detail);
}

void ListAndDetailViewWidget::updateListViewCurrentDevice(const DeviceDetailViewData & data) noexcept
{
  qDebug() << "updateListViewCurrentDevice() " << data.id << ", " << data.description;

  const int row = mListViewSortFilterModel.mapToSource( listViewCurrentIndex() ).row();

  qDebug() << "  row: " << row;

  if(row < 0){
    fetchAllDevices();
  }else{
    DeviceListRecord record;
    record.id = data.id.toInt();
    record.description = data.description.toStdString();

    mListViewModel.setRecord(row, record);
  }
}

void ListAndDetailViewWidget::setEditorState(const EditorState & state) noexcept
{
  qDebug() << "Editor state changed to " << state.debugName();
  qDebug() << " can change current device: " << state.canChangeCurrentDevice();
  qDebug() << " has things to save: " << state.haveSomethingToSave();

  mUi.pbPrepend->setEnabled( state.canChangeCurrentDevice() );
  mUi.pbAppend->setEnabled( state.canChangeCurrentDevice() );
  mUi.pbResetModel->setEnabled( state.canChangeCurrentDevice() );
  mUi.pbRemove->setEnabled( state.canChangeCurrentDevice() );
  mUi.pbFilter->setEnabled( state.canChangeCurrentDevice() );
  mUi.filterCriteria->setEnabled( state.canChangeCurrentDevice() );

  if(mListViewMdtSelectionModel){
    mListViewMdtSelectionModel->setChangeCurrentRowAllowed( state.canChangeCurrentDevice() );
  }

  mUi.pbCancelChanges->setEnabled( state.haveSomethingToSave() );
  mUi.pbSave->setEnabled( state.haveSomethingToSave() );

  mUi.pbDelete->setEnabled( state.canDeleteDevice() );
}

void ListAndDetailViewWidget::displayListViewCurrentChangedEvent(const QModelIndex & current) noexcept
{
  QString currentIndexText = QString::number( current.row() ) + QLatin1Char(',') + QString::number( current.column() );
  QString text = tr("Current index: %1 ").arg(currentIndexText);
  mUi.currentChangedEventLabel->setText(text);
  mResetDisplayListViewCurrentChangedEventTimer.start();
}

void ListAndDetailViewWidget::resetDisplayListViewCurrentChangedEvent() noexcept
{
  mResetDisplayListViewCurrentChangedEventTimer.stop();
  mUi.currentChangedEventLabel->clear();
}

void ListAndDetailViewWidget::prependItem() noexcept
{
  auto *model = mUi.tableView->model();
  assert(model != nullptr);

  prependRowToModel(*model);
}

void ListAndDetailViewWidget::appendItem() noexcept
{
  auto *model = mUi.tableView->model();
  assert(model != nullptr);

  appendRowToModel(*model);
}

void ListAndDetailViewWidget::removeSelectedItems() noexcept
{
  removeSelectedRows(*mUi.tableView);
}

void ListAndDetailViewWidget::setCurrentDeviceFromListViewRow(const QModelIndex & current, const QModelIndex & previous)
{
  qDebug() << "\nsetCurrentDeviceFromListViewRow() - view row: " << current.row() << "\n previous: " << previous;

  const QModelIndex index = mListViewSortFilterModel.mapToSource(current);

  const int row = index.row();
  const int column = DeviceListTableModel::idColumn();

  QVariant id;

  // row can be out of bound when editing a new device
  if( modelRowAndColumnAreInRange(mListViewModel, row, column) ){
    id = getModelData(mListViewModel, row, column);
  }

  mEditor.setCurrentDevice(id);
}

void ListAndDetailViewWidget::applyFilter() noexcept
{
  assert( mListViewSortFilterModel.sourceModel() != nullptr );

  QLineEdit *lineEdit = mUi.filterCriteria;
  assert(lineEdit != nullptr);

  const QString pattern = QRegularExpression::wildcardToRegularExpression( lineEdit->text() );

  QRegularExpression regularExpression(pattern);

  if( regularExpression.isValid() ){
    lineEdit->setToolTip( QString() );
    mListViewSortFilterModel.setFilterRegularExpression(regularExpression);
  }else{
    lineEdit->setToolTip( regularExpression.errorString() );
    mListViewSortFilterModel.setFilterRegularExpression( QRegularExpression() );
  }
}

void ListAndDetailViewWidget::fetchAllDevices()
{
  qDebug() << "fetch all devices ...";
  mListViewModel.setTable( mDeviceLibrary->fetchAll() );
}

void ListAndDetailViewWidget::saveDevice()
{
  DeviceDetailViewData data;

  data.id = mUi.id->text();
  data.description = mUi.description->text();
  data.detail = mUi.detail->text();

  mEditor.saveChanges(data);
}

void ListAndDetailViewWidget::deleteDevice()
{
  auto *model = mUi.tableView->model();
  assert(model != nullptr);

  const int row = listViewCurrentIndex().row();
  assert(row >= 0);

  mEditor.deleteDevice();

  model->removeRow(row);
}

void ListAndDetailViewWidget::setQtSelectionModelIf(bool checked) noexcept
{
  if(!checked){
    return;
  }
  assert(mListViewQtSelectionModel.get() == nullptr);

  disconnectItemSelectionModelSignals();
  mListViewMdtSelectionModel.reset();

  mListViewQtSelectionModel = std::make_unique<QItemSelectionModel>();
  mListViewQtSelectionModel->setModel(&mListViewSortFilterModel);
  mUi.tableView->setSelectionModel( mListViewQtSelectionModel.get() );

  connectItemSelectionModelSignals();
}

void ListAndDetailViewWidget::setMdtSelectionModelIf(bool checked) noexcept
{
  if(!checked){
    return;
  }
  assert(mListViewMdtSelectionModel.get() == nullptr);

  disconnectItemSelectionModelSignals();
  mListViewQtSelectionModel.reset();

  mListViewMdtSelectionModel = std::make_unique<ItemSelectionModel>();
  mListViewMdtSelectionModel->setCurrentIndexToFirstRowAfterReset(true);
  mListViewMdtSelectionModel->setModel(&mListViewSortFilterModel);
  mUi.tableView->setSelectionModel( mListViewMdtSelectionModel.get() );

  connectItemSelectionModelSignals();
}

void ListAndDetailViewWidget::connectItemSelectionModelSignals() noexcept
{
  assert( mUi.tableView->selectionModel() != nullptr );

  connect(mUi.tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &ListAndDetailViewWidget::setCurrentDeviceFromListViewRow);
  connect(mUi.tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &ListAndDetailViewWidget::displayListViewCurrentChangedEvent);
}

void ListAndDetailViewWidget::disconnectItemSelectionModelSignals() noexcept
{
  assert( mUi.tableView->selectionModel() != nullptr );

  disconnect(mUi.tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &ListAndDetailViewWidget::setCurrentDeviceFromListViewRow);
  disconnect(mUi.tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &ListAndDetailViewWidget::displayListViewCurrentChangedEvent);
}

QModelIndex ListAndDetailViewWidget::listViewCurrentIndex() const noexcept
{
  auto *selectionModel = mUi.tableView->selectionModel();
  assert(selectionModel != nullptr);

  return selectionModel->currentIndex();
}
