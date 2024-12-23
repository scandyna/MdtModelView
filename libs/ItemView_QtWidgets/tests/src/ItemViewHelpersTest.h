// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef ITEM_VIEW_HELPERS_TEST_H
#define ITEM_VIEW_HELPERS_TEST_H

#include <QTest>
#include <QObject>

class ItemViewHelpersTest : public QObject
{
  Q_OBJECT

 private slots:

//   void removeSelectedRows_Sandbox();
  void removeSelectedRows_SelectAll();
};

#endif // #ifndef ITEM_VIEW_HELPERS_TEST_H
