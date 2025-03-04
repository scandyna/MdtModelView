// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "Mdt/ItemModel/StlContiguousContainerFunctionMap.h"
#include "Mdt/ItemModel/StlContiguousContainerAdapter.h"
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include <vector>

using namespace Mdt::ItemModel;


TEST_CASE("StdVector_Adapter_Test")
{
  using List = std::vector<int>;
  using ListAdapted = StlContiguousContainerAdapter< List, StlContiguousContainerFunctionMap<List> >;

  ListAdapted list;
  CHECK( list.rowCount() == 0 );

  list.insertRows(0, 1, 100);
  CHECK( list.rowCount() == 1 );

  CHECK( list.atRow(0) == 100 );

  list.atRowMutable(0) = 250;
  CHECK( list.atRow(0) == 250 );

  list.removeRows(0, 1);
  CHECK( list.rowCount() == 0 );
}
