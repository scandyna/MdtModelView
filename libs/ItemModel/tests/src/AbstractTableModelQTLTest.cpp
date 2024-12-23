// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "AbstractTableModelQTLTest.h"
#include "ReadOnlyTableModel.h"
#include "EditableTableModel.h"
#include "AppendRowTableModel.h"
#include "RemoveRowsTableModel.h"
#include <QAbstractItemModelTester>

using namespace Mdt::ItemModel::TestLib;


void populateModel(ReadOnlyTableModel & model, const ReadOnlyTableModel::Table & tableData)
{
  model.setTable(tableData);
}

void populateModel(EditableTableModel & model, const EditableTableModel::Table & tableData)
{
  model.setTable(tableData);
}

void populateModel(TableModelCommonBase & model, const TableModelCommonBase::Table & tableData)
{
  model.setTable(tableData);
}


void AbstractTableModelQTLTest::itemModelTester_ReadOnly_empty()
{
  ReadOnlyTableModel model;

  QAbstractItemModelTester tester(&model);
}

void AbstractTableModelQTLTest::itemModelTester_ReadOnly_1row()
{
  ReadOnlyTableModel model;
  populateModel(model, {{1,"A"}});

  QAbstractItemModelTester tester(&model);
}

void AbstractTableModelQTLTest::itemModelTester_ReadOnly_3rows()
{
  ReadOnlyTableModel model;
  populateModel(model, {{1,"A"},{2,"B"},{3,"C"}});

  QAbstractItemModelTester tester(&model);
}

void AbstractTableModelQTLTest::itemModelTester_Editable_empty()
{
  EditableTableModel model;

  QAbstractItemModelTester tester(&model);
}

void AbstractTableModelQTLTest::itemModelTester_Editable_1row()
{
  EditableTableModel model;
  populateModel(model, {{1,"A"}});

  QAbstractItemModelTester tester(&model);
}

void AbstractTableModelQTLTest::itemModelTester_Editable_3rows()
{
  EditableTableModel model;
  populateModel(model, {{1,"A"},{2,"B"},{3,"C"}});

  QAbstractItemModelTester tester(&model);
}

void AbstractTableModelQTLTest::itemModelTester_AppendRow_empty()
{
  AppendRowTableModel model;

  QAbstractItemModelTester tester(&model);
}

void AbstractTableModelQTLTest::itemModelTester_RemoveRows_3rows()
{
  RemoveRowsTableModel model;
  populateModel(model, {{1,"A"},{2,"B"},{3,"C"}});
  QCOMPARE( model.rowCount(), 3 );

  QAbstractItemModelTester tester(&model);
}

QTEST_GUILESS_MAIN(AbstractTableModelQTLTest)
