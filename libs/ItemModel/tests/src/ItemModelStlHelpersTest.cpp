// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "Mdt/ItemModel/StlHelpers.h"
#include "Mdt/ItemModel/NumericLimits.h"
#include <vector>

using namespace Mdt::ItemModel;

struct TestVector : public std::vector<int>
{
  void setMaxSize(size_type s) noexcept
  {
    mMaxSize = s;
  }

  size_type max_size() const noexcept
  {
    return mMaxSize;
  }

 private:

  size_type mMaxSize = 1;
};


TEST_CASE("stlContainerMaxElementCount")
{
  TestVector v;
  v.setMaxSize(25);
  REQUIRE( v.max_size() == 25 );

  CHECK( stlContainerMaxElementCount(v) == 25 );
}

TEST_CASE("stlContainerElementCount")
{
  SECTION("empty collection")
  {
    std::vector<int> v;

    CHECK( stlContainerElementCount(v) == 0 );
  }

  SECTION("collection with 3 elements")
  {
    std::vector<int> v{1,2,3};

    CHECK( stlContainerElementCount(v) == 3 );
  }
}

TEST_CASE("canAddCountElementsToStlContainer")
{
  TestVector v;
  v.setMaxSize(5);
  REQUIRE( v.max_size() == 5 );

  SECTION("empty collection")
  {
    REQUIRE( v.size() == 0 );

    CHECK( canAddCountElementsToStlContainer(v, 1) );
    CHECK( canAddCountElementsToStlContainer(v, 2) );
    CHECK( canAddCountElementsToStlContainer(v, 3) );
    CHECK( canAddCountElementsToStlContainer(v, 4) );
    CHECK( canAddCountElementsToStlContainer(v, 5) );
    CHECK( !canAddCountElementsToStlContainer(v, 6) );
    CHECK( !canAddCountElementsToStlContainer(v, 7) );
    CHECK( !canAddCountElementsToStlContainer( v, intMax() ) );
  }

  SECTION("collection with 2 elements")
  {
    insertToStlContainer(v, 0, 2, 46);
    REQUIRE( v.size() == 2 );

    CHECK( canAddCountElementsToStlContainer(v, 1) );
    CHECK( canAddCountElementsToStlContainer(v, 2) );
    CHECK( canAddCountElementsToStlContainer(v, 3) );
    CHECK( !canAddCountElementsToStlContainer(v, 4) );
    CHECK( !canAddCountElementsToStlContainer(v, 5) );
    CHECK( !canAddCountElementsToStlContainer( v, intMax() ) );
  }
}

TEST_CASE("positionIsInRangeOfStlContainer")
{
  std::vector<int> v{1,2,3};
  REQUIRE( v.size() == 3 );

  CHECK( positionIsInRangeOfStlContainer( v, v.cbegin() ) );
  CHECK( positionIsInRangeOfStlContainer( v, v.cbegin()+1 ) );
  CHECK( positionIsInRangeOfStlContainer( v, v.cbegin()+2 ) );
  CHECK( !positionIsInRangeOfStlContainer( v, v.cbegin()+3 ) );
  CHECK( !positionIsInRangeOfStlContainer( v, v.cend() ) );
}

TEST_CASE("indexFromPositionInStlContainer")
{
  std::vector<int> v{1,2,3};
  REQUIRE( v.size() == 3 );

  CHECK( indexFromPositionInStlContainer( v, v.cbegin() ) == 0 );
  CHECK( indexFromPositionInStlContainer( v, v.cbegin()+1 ) == 1 );
  CHECK( indexFromPositionInStlContainer( v, v.cbegin()+2 ) == 2 );
}

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
