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
#include "Mdt/ItemModel/StlHelpers.h"
#include <vector>

using namespace Mdt::ItemModel;


TEST_CASE("insertToStlContainer")
{
  SECTION("insert to an empty container")
  {
    std::vector<int> v;

    SECTION("1 element")
    {
      insertToStlContainer(v, 0, 1, 10);

      REQUIRE( v.size() == 1 );
      REQUIRE( v[0] == 10 );
    }

    SECTION("2 elements")
    {
      insertToStlContainer(v, 0, 2, 20);

      REQUIRE( v.size() == 2 );
      REQUIRE( v[0] == 20 );
      REQUIRE( v[1] == 20 );
    }
  }

  SECTION("insert to a container with 1 element")
  {
    std::vector<int> v{1};

    SECTION("insert 1 element before")
    {
      insertToStlContainer(v, 0, 1, -1);

      REQUIRE( v.size() == 2 );
      REQUIRE( v[0] == -1 );
      REQUIRE( v[1] == 1 );
    }

    SECTION("insert 1 element after")
    {
      insertToStlContainer(v, 1, 1, 2);

      REQUIRE( v.size() == 2 );
      REQUIRE( v[0] == 1 );
      REQUIRE( v[1] == 2 );
    }
  }
}

TEST_CASE("removeFromStlContainer")
{
  std::vector<int> v{1,2,3};

  SECTION("remove first element")
  {
    removeFromStlContainer(v, 0, 1);

    REQUIRE( v.size() == 2 );
    REQUIRE( v[0] == 2 );
    REQUIRE( v[1] == 3 );
  }

  SECTION("remove last element")
  {
    removeFromStlContainer(v, 2, 1);

    REQUIRE( v.size() == 2 );
    REQUIRE( v[0] == 1 );
    REQUIRE( v[1] == 2 );
  }

  SECTION("remove all elements")
  {
    removeFromStlContainer(v, 0, 3);

    REQUIRE( v.size() == 0 );
  }
}

TEST_CASE("removeFirstFromStlContainer")
{
  std::vector<int> v{1,2,3};

  removeFirstFromStlContainer(v);

  REQUIRE( v.size() == 2 );
  REQUIRE( v[0] == 2 );
  REQUIRE( v[1] == 3 );
}

TEST_CASE("removeLastFromStlContainer")
{
  std::vector<int> v{1,2,3};

  removeLastFromStlContainer(v);

  REQUIRE( v.size() == 2 );
  REQUIRE( v[0] == 1 );
  REQUIRE( v[1] == 2 );
}
