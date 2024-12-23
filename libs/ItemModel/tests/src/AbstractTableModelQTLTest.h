// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef ABSTRACT_TABLE_MODEL_QTL_TEST_H
#define ABSTRACT_TABLE_MODEL_QTL_TEST_H

#include <QTest>
#include <QObject>

class AbstractTableModelQTLTest : public QObject
{
  Q_OBJECT

 private slots:

  void itemModelTester_ReadOnly_empty();
  void itemModelTester_ReadOnly_1row();
  void itemModelTester_ReadOnly_3rows();

  void itemModelTester_Editable_empty();
  void itemModelTester_Editable_1row();
  void itemModelTester_Editable_3rows();

  void itemModelTester_AppendRow_empty();

  void itemModelTester_RemoveRows_3rows();
};

#endif // #ifndef ABSTRACT_TABLE_MODEL_QTL_TEST_H
