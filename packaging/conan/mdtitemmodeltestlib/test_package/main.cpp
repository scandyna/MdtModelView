// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2024-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#include <Mdt/ItemModel/TestLib/TableModelCommonBase.h>
#include <iostream>

int main()
{
  Mdt::ItemModel::TestLib::TableModelCommonBase model;
  model.setTable({{1,"A"},{2,"B"}});

  std::cout << " model rows: " << model.rowCount() << std::endl;

  return 0;
}
