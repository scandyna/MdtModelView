// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ItemSelectionModelTestWidget.h"
#include "Mdt/ItemModel/Helpers.h"
#include "Mdt/ItemView/Helpers.h"
#include <QItemSelectionModel>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QRegularExpression>
#include <QPalette>
#include <QColor>
#include <cassert>

using namespace Mdt::ItemModel;

/*
 * textColor() and setTextColor()
 * comes from basic sort/filter example
 * https://doc.qt.io/qt-6/qtwidgets-itemviews-basicsortfiltermodel-example.html
 */

static
inline
QColor textColor(const QPalette &palette)
{
  return palette.color(QPalette::Active, QPalette::Text);
}

static void setTextColor(QWidget *w, const QColor &c)
{
  auto palette = w->palette();

  if(textColor(palette) != c){
    palette.setColor(QPalette::Active, QPalette::Text, c);
    w->setPalette(palette);
  }
}


ItemSelectionModelTestWidget::ItemSelectionModelTestWidget(QWidget *parent)
 : QWidget(parent)
{
  mUi.setupUi(this);

  connect(mUi.useSortFilterProxyModel, &QCheckBox::stateChanged, this, &ItemSelectionModelTestWidget::useSortFilterProfyModelIf);

  mUi.useSortFilterProxyModel->setChecked(true);

  connect(mUi.rbQtSelectionModel, &QRadioButton::toggled, this, &ItemSelectionModelTestWidget::setQtSelectionModelIf);
  connect(mUi.rbMdtSelectionModel, &QRadioButton::toggled, this, &ItemSelectionModelTestWidget::setMdtSelectionModelIf);
  mUi.rbQtSelectionModel->setChecked(true);

  connect(mUi.pbResetModel, &QPushButton::clicked, this, &ItemSelectionModelTestWidget::populateModel);

  connect(mUi.pbPrepend, &QPushButton::clicked, this, &ItemSelectionModelTestWidget::prependItem);
  connect(mUi.pbAppend, &QPushButton::clicked, this, &ItemSelectionModelTestWidget::appendItem);
  connect(mUi.pbRemove, &QPushButton::clicked, this, &ItemSelectionModelTestWidget::removeSelectedItems);

  connect(mUi.pbDisplayCurrentIndex, &QPushButton::clicked, this, &ItemSelectionModelTestWidget::displayAllCurrentIndex);

  mUi.filterCriteria->setText( QLatin1String("*") );
  connect(mUi.pbFilter, &QPushButton::clicked, this, &ItemSelectionModelTestWidget::applyFilter);

  mTableViewCurrentChangedDisplayResetTimer.setSingleShot(true);
  mTableViewCurrentChangedDisplayResetTimer.setInterval(500);
  mTableViewCurrentChangedDisplayResetTimer.setTimerType(Qt::CoarseTimer);
  connect(&mTableViewCurrentChangedDisplayResetTimer, &QTimer::timeout, this, &ItemSelectionModelTestWidget::resetDisplayTableViewCurrentChangedEmitted);

  populateModel();
}

void ItemSelectionModelTestWidget::setQtSelectionModelIf(bool checked) noexcept
{
  if(!checked){
    return;
  }

  disconnectItemSelectionModelsSignals();

  mUi.tableView->setSelectionModel(&mTableViewQtSelectionModel);
  mUi.listView->setSelectionModel(&mListViewQtSelectionModel);
  mUi.treeView->setSelectionModel(&mTreeViewQtSelectionModel);

  connectItemSelectionModelsSignals();
}

void ItemSelectionModelTestWidget::setMdtSelectionModelIf(bool checked) noexcept
{
  if(!checked){
    return;
  }

  disconnectItemSelectionModelsSignals();

  mUi.tableView->setSelectionModel(&mTableViewMdtSelectionModel);
  mUi.listView->setSelectionModel(&mListViewMdtSelectionModel);
  mUi.treeView->setSelectionModel(&mTreeViewMdtSelectionModel);

  connectItemSelectionModelsSignals();
}

void ItemSelectionModelTestWidget::useSortFilterProfyModelIf(int checkState) noexcept
{
  if(mUi.tableView->selectionModel() != nullptr){
    disconnectItemSelectionModelsSignals();
  }

  if(checkState == Qt::Checked){
    mTableViewSortFilterModel.setSourceModel(&mTableModel);
    mTableViewSortFilterModel.setFilterKeyColumn(1);

    mListViewSortFilterModel.setSourceModel(&mTableModel);
    mListViewSortFilterModel.setFilterKeyColumn(1);

    mTreeViewSortFilterModel.setSourceModel(&mTableModel);
    mTreeViewSortFilterModel.setFilterKeyColumn(1);

    mUi.tableView->setModel(&mTableViewSortFilterModel);
    mUi.tableView->setSortingEnabled(true);

    mUi.listView->setModel(&mListViewSortFilterModel);

    mUi.treeView->setModel(&mTreeViewSortFilterModel);
    mUi.treeView->setSortingEnabled(true);

    mTableViewQtSelectionModel.setModel(&mTableViewSortFilterModel);
    mListViewQtSelectionModel.setModel(&mListViewSortFilterModel);
    mTreeViewQtSelectionModel.setModel(&mTreeViewSortFilterModel);

    mTableViewMdtSelectionModel.setModel(&mTableViewSortFilterModel);
    mListViewMdtSelectionModel.setModel(&mListViewSortFilterModel);
    mTreeViewMdtSelectionModel.setModel(&mTreeViewSortFilterModel);

    mUi.pbFilter->setEnabled(true);
    mUi.filterCriteria->setEnabled(true);
  }else{
    mTableViewSortFilterModel.setSourceModel(&mFakeTableModel);
    mListViewSortFilterModel.setSourceModel(&mFakeTableModel);
    mTreeViewSortFilterModel.setSourceModel(&mFakeTableModel);

    mUi.tableView->setModel(&mTableModel);
    mUi.tableView->setSortingEnabled(false);

    mUi.listView->setModel(&mTableModel);

    mUi.treeView->setModel(&mTableModel);
    mUi.treeView->setSortingEnabled(false);

    mTableViewQtSelectionModel.setModel(&mTableModel);
    mListViewQtSelectionModel.setModel(&mTableModel);
    mTreeViewQtSelectionModel.setModel(&mTableModel);

    mTableViewMdtSelectionModel.setModel(&mTableModel);
    mListViewMdtSelectionModel.setModel(&mTableModel);
    mTreeViewMdtSelectionModel.setModel(&mTableModel);

    mUi.pbFilter->setEnabled(false);
    mUi.filterCriteria->setEnabled(false);
  }

  connectItemSelectionModelsSignals();
}

void ItemSelectionModelTestWidget::populateModel() noexcept
{
  mTableModel.setTable({{1,"A"},{2,"B"}});
}

void ItemSelectionModelTestWidget::prependItem() noexcept
{
  /// \todo should act on view's model
  prependRowToModel(mTableModel);
}

void ItemSelectionModelTestWidget::appendItem() noexcept
{
  /// \todo should act on view's model
  appendRowToModel(mTableModel);
}

void ItemSelectionModelTestWidget::removeSelectedItems() noexcept
{
  Mdt::ItemView::removeSelectedRows(*mUi.tableView);
}

void ItemSelectionModelTestWidget::displayTableViewCurrentIndex(const QModelIndex & current) noexcept
{
  mUi.tableViewCurrentIndex->setText( formatIndex(current) );
}

void ItemSelectionModelTestWidget::displayTableViewCurrentChangedEmitted() noexcept
{
  mUi.tableViewCurrentChangedEmitted->setText( tr("NOW") );
  mTableViewCurrentChangedDisplayResetTimer.start();
}

void ItemSelectionModelTestWidget::resetDisplayTableViewCurrentChangedEmitted() noexcept
{
  mTableViewCurrentChangedDisplayResetTimer.stop();
  mUi.tableViewCurrentChangedEmitted->clear();
}

void ItemSelectionModelTestWidget::displayListViewCurrentIndex(const QModelIndex & current) noexcept
{
  mUi.listViewCurrentIndex->setText( formatIndex(current) );
}

void ItemSelectionModelTestWidget::displayTreeViewCurrentIndex(const QModelIndex & current) noexcept
{
  mUi.treeViewCurrentIndex->setText( formatIndex(current) );
}

void ItemSelectionModelTestWidget::displayAllCurrentIndex() noexcept
{
  displayTableViewCurrentIndex( mUi.tableView->currentIndex() );
  displayListViewCurrentIndex( mUi.listView->currentIndex() );
  displayTreeViewCurrentIndex( mUi.treeView->currentIndex() );
}

void ItemSelectionModelTestWidget::connectItemSelectionModelsSignals() noexcept
{
  assert( mUi.tableView->selectionModel() != nullptr );
  assert( mUi.listView->selectionModel() != nullptr );
  assert( mUi.treeView->selectionModel() != nullptr );

  connect(mUi.tableView->selectionModel(), &QItemSelectionModel::currentChanged, this, &ItemSelectionModelTestWidget::displayTableViewCurrentIndex);
  connect(mUi.tableView->selectionModel(), &QItemSelectionModel::currentChanged, this, &ItemSelectionModelTestWidget::displayTableViewCurrentChangedEmitted);
  connect(mUi.listView->selectionModel(), &QItemSelectionModel::currentChanged, this, &ItemSelectionModelTestWidget::displayListViewCurrentIndex);
  connect(mUi.treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &ItemSelectionModelTestWidget::displayTreeViewCurrentIndex);
}

void ItemSelectionModelTestWidget::disconnectItemSelectionModelsSignals() noexcept
{
  assert( mUi.tableView->selectionModel() != nullptr );
  assert( mUi.listView->selectionModel() != nullptr );
  assert( mUi.treeView->selectionModel() != nullptr );

  disconnect(mUi.tableView->selectionModel(), &QItemSelectionModel::currentChanged, this, &ItemSelectionModelTestWidget::displayTableViewCurrentIndex);
  disconnect(mUi.tableView->selectionModel(), &QItemSelectionModel::currentChanged, this, &ItemSelectionModelTestWidget::displayTableViewCurrentChangedEmitted);
  disconnect(mUi.listView->selectionModel(), &QItemSelectionModel::currentChanged, this, &ItemSelectionModelTestWidget::displayListViewCurrentIndex);
  disconnect(mUi.treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &ItemSelectionModelTestWidget::displayTreeViewCurrentIndex);
}

void ItemSelectionModelTestWidget::applyFilter() noexcept
{
  assert( mTableViewSortFilterModel.sourceModel() != nullptr );
  assert( mListViewSortFilterModel.sourceModel() != nullptr );
  assert( mTreeViewSortFilterModel.sourceModel() != nullptr );

  QLineEdit *lineEdit = mUi.filterCriteria;
  assert(lineEdit != nullptr);

  const QString pattern = QRegularExpression::wildcardToRegularExpression( lineEdit->text() );

  QRegularExpression regularExpression(pattern);

  if( regularExpression.isValid() ){
    lineEdit->setToolTip( QString() );
    mTableViewSortFilterModel.setFilterRegularExpression(regularExpression);
    mListViewSortFilterModel.setFilterRegularExpression(regularExpression);
    mTreeViewSortFilterModel.setFilterRegularExpression(regularExpression);
    setTextColor( lineEdit, textColor( style()->standardPalette() ) );
  }else{
    lineEdit->setToolTip( regularExpression.errorString() );
    mTableViewSortFilterModel.setFilterRegularExpression( QRegularExpression() );
    mListViewSortFilterModel.setFilterRegularExpression( QRegularExpression() );
    mTreeViewSortFilterModel.setFilterRegularExpression( QRegularExpression() );
    setTextColor(lineEdit, Qt::red);
  }
}

QString ItemSelectionModelTestWidget::formatIndex(const QModelIndex & index) noexcept
{
  return QString::fromLatin1("%1, %2").arg( index.row() ).arg( index.column() );
}
