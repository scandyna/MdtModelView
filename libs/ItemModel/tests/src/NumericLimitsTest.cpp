// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "Mdt/ItemModel/NumericLimits.h"

using namespace Mdt::ItemModel;


TEST_CASE("isIntMax")
{
  SECTION("1000 is not int max")
  {
    REQUIRE( !isIntMax(1000) );
  }

  SECTION("int max")
  {
    REQUIRE( isIntMax( intMax() ) );
  }
}
