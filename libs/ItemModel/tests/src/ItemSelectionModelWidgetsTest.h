// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef ITEM_SELECTION_MODEL_WIDGETS_TEST_H
#define ITEM_SELECTION_MODEL_WIDGETS_TEST_H

#include "Mdt/ItemModel/ItemSelectionModel.h"
#include <QTest>
#include <QObject>


class ItemSelectionModelWidgetsTest : public QObject
{
  Q_OBJECT

 public:

  explicit
  ItemSelectionModelWidgetsTest(QObject *parent = nullptr);

 private slots:

  void initTestCase();
  void cleanup();

  void tableViewTest();
  void tableViewSelectRowsTest();
  void listViewTest();
  void treeViewTest();

  void modelReset();

  void currentIndexIsSetWithCorrectRowAndColumn();
};

#endif // #ifndef ITEM_SELECTION_MODEL_WIDGETS_TEST_H
