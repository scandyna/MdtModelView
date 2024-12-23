// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef ITEM_SELECTION_MODEL_TESTER_H
#define ITEM_SELECTION_MODEL_TESTER_H

#include "Mdt/ItemModel/ItemSelectionModel.h"
#include <QObject>
#include <QPointer>
#include <QModelIndex>
#include <cassert>

class ItemSelectionModelTester : public QObject
{
  Q_OBJECT

 public:

  explicit
  ItemSelectionModelTester(Mdt::ItemModel::ItemSelectionModel *selectionModel);

  size_t currentRowChangedCount() const noexcept
  {
    return mCurrentRowChangedCount;
  }

  void clearCounters() noexcept;

 private slots:

  void appendCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous);

 private:

  QPointer<Mdt::ItemModel::ItemSelectionModel> mSelectionModel;
  size_t mCurrentRowChangedCount = 0;
};

#endif // #ifndef ITEM_SELECTION_MODEL_TESTER_H
