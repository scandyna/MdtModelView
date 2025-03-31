// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "Mdt/ItemModel/StlContiguousContainerAdapter.h"
#include "catch2/catch.hpp"
#include "Catch2QString.h"
// #include <catch2/catch_template_test_macros.hpp>
#include "Item.h"
#include "DefaultConstructibleOnlyList.h"
#include "CopyConstructibleOnlyList.h"
#include "MoveConstructibleOnlyList.h"
#include "ReadOnlyList.h"
#include "ReadOnlyListTableModelAdapterFunctionMap.h"
#include "DefaultConstructibleOnlyListTableModelAdapterFunctionMap.h"
#include "CopyConstructibleOnlyListTableModelAdapterFunctionMap.h"
#include "MoveConstructibleOnlyListTableModelAdapterFunctionMap.h"
#include "MutableListRawData.h"
#include "MutableListRawDataTableModelAdapterFunctionMap.h"
#include "MutableList.h"
#include "MutableListTableModelAdapterFunctionMap.h"
#include "ListWithInsert.h"
#include "ListWithInsertTableModelAdapterFunctionMap.h"
#include "ListWithAppend.h"
#include "ListWithAppendTableModelAdapterFunctionMap.h"
#include "ListWithErase.h"
#include "ListWithEraseTableModelAdapterFunctionMap.h"
#include "ReadOnlyWithIteratorFindList.h"
#include "ReadOnlyWithIteratorFindListTableModelAdapterFunctionMap.h"
#include "ListWithInsertAndLimit.h"
#include "ListWithInsertAndLimitTableModelAdapterFunctionMap.h"

using namespace Mdt::ItemModel;


/*
 * TEMPLATE_TEST_CASE_SIG() does not work for our case.
 * See https://github.com/catchorg/Catch2/issues/2680
 */
template<typename ContainerType, typename FunctionMapType>
struct ContainerAndFunctionMap
{
  using Container = ContainerType;
  using FunctionMap = FunctionMapType;
};


using DefaultConstructibleOnlyListAdapted = StlContiguousContainerAdapter<DefaultConstructibleOnlyList, DefaultConstructibleOnlyListTableModelAdapterFunctionMap>;
using CopyConstructibleOnlyListAdapted = StlContiguousContainerAdapter<CopyConstructibleOnlyList, CopyConstructibleOnlyListTableModelAdapterFunctionMap>; 
using MoveConstructibleOnlyListAdapted = StlContiguousContainerAdapter<MoveConstructibleOnlyList, MoveConstructibleOnlyListTableModelAdapterFunctionMap>;

using ReadOnlyListContainerAndFunctionMap = ContainerAndFunctionMap<ReadOnlyList, ReadOnlyListTableModelAdapterFunctionMap>;
using ReadOnlyListAdapted = StlContiguousContainerAdapter<ReadOnlyList, ReadOnlyListTableModelAdapterFunctionMap>;

using MutableListRawDataAdapted = StlContiguousContainerAdapter<MutableListRawData, MutableListRawDataTableModelAdapterFunctionMap>;
using MutableListAdapted = StlContiguousContainerAdapter<MutableList, MutableListTableModelAdapterFunctionMap>;

using ListWithInsertAdapted = StlContiguousContainerAdapter<ListWithInsert, ListWithInsertTableModelAdapterFunctionMap>;
using ListWithAppendAdapted = StlContiguousContainerAdapter<ListWithAppend, ListWithAppendTableModelAdapterFunctionMap>;

using ListWithInsertAndLimitAdapted = StlContiguousContainerAdapter<ListWithInsertAndLimit, ListWithInsertAndLimitTableModelAdapterFunctionMap>;

using ListWithEraseAdapted = StlContiguousContainerAdapter<ListWithErase, ListWithEraseTableModelAdapterFunctionMap>;

using ReadOnlyWithIteratorFindListAdapted = StlContiguousContainerAdapter<ReadOnlyWithIteratorFindList, ReadOnlyWithIteratorFindListTableModelAdapterFunctionMap>;


TEMPLATE_TEST_CASE("default_constructed", "", DefaultConstructibleOnlyListAdapted)
{
  TestType list;

  CHECK( list.rowCount() == 0 );
}

TEST_CASE("copy_constructed")
{
  SECTION("CopyConstructibleOnlyList")
  {
    CopyConstructibleOnlyList list = CopyConstructibleOnlyList::fromItemList({{1,"A"}});

    CopyConstructibleOnlyListAdapted listAdapted(list);

    CHECK( listAdapted.rowCount() == 1 );
  }
}

TEST_CASE("move_constructed")
{
  SECTION("MoveConstructibleOnlyList")
  {
    MoveConstructibleOnlyListAdapted list( MoveConstructibleOnlyList::fromItemList({{1,"A"}}) );

    CHECK( list.rowCount() == 1 );
  }

  SECTION("ReadOnlyList")
  {
    ReadOnlyListAdapted list( ReadOnlyList::fromItemList({{1,"A"}}) );

    CHECK( list.rowCount() == 1 );
  }
}

TEST_CASE("indexFromRow")
{
  ReadOnlyListAdapted list( ReadOnlyList::fromItemList({{1,"A"}}) );
  REQUIRE( list.rowCount() == 1 );

  CHECK( list.indexFromRow(0) == 0 );
}

TEST_CASE("rowFromIndex")
{
  ReadOnlyListAdapted list( ReadOnlyList::fromItemList({{1,"A"},{2,"B"}}) );
  REQUIRE( list.rowCount() == 2 );

  CHECK( list.rowFromIndex(0) == 0 );
  CHECK( list.rowFromIndex(1) == 1 );
}

TEST_CASE("positionIsInRange")
{
  ReadOnlyWithIteratorFindListAdapted list( ReadOnlyWithIteratorFindList::fromItemList({{1,"A"},{2,"B"},{3,"C"}}) );
  REQUIRE( list.rowCount() == 3 );

  CHECK( list.positionIsInRange( list.container().cbegin() ) );
  CHECK( list.positionIsInRange( list.container().cbegin()+1 ) );
  CHECK( list.positionIsInRange( list.container().cbegin()+2 ) );
  CHECK( !list.positionIsInRange( list.container().cbegin()+3 ) );
  CHECK( !list.positionIsInRange( list.container().cend() ) );
}

TEST_CASE("rowFromPosition")
{
  ReadOnlyWithIteratorFindListAdapted list( ReadOnlyWithIteratorFindList::fromItemList({{1,"A"},{2,"B"},{3,"C"}}) );
  REQUIRE( list.rowCount() == 3 );

  CHECK( list.rowFromPosition( list.container().cbegin() ) == 0 );
  CHECK( list.rowFromPosition( list.container().cbegin()+1 ) == 1 );
  CHECK( list.rowFromPosition( list.container().cbegin()+2 ) == 2 );
}

TEMPLATE_TEST_CASE("ReadOnly_example", "", ReadOnlyListContainerAndFunctionMap)
{
  using Container = typename TestType::Container;
  using List = StlContiguousContainerAdapter<Container, typename TestType::FunctionMap>;

  List list( Container::fromItemList({{1,"A"}}) );

  CHECK( list.rowCount() == 1 );
  CHECK( list.atRow(0).id == 1 );
}

TEST_CASE("MutableListRawData_example")
{
  MutableListRawDataAdapted list( MutableListRawData::fromItemList({{1,"A"}}) );
  REQUIRE( list.rowCount() == 1 );
  REQUIRE( list.atRow(0).name == "A" );

  list.atRowMutable(0).name = "B";

  CHECK( list.atRow(0).name == "B" );
}

TEST_CASE("MutableList_DomainMethods_example")
{
  MutableListAdapted list( MutableList::fromItemList({{1,"A"}}) );
  REQUIRE( list.rowCount() == 1 );
  REQUIRE( list.atRow(0).name == "A" );

  const MutableList::size_type containerIndex = list.indexFromRow(0);
  list.containerMutable().setNameAt(containerIndex, "C");

  CHECK( list.atRow(0).name == "C" );
}

TEST_CASE("ListWithInsert_example")
{
  ListWithInsertAdapted list;
  REQUIRE( list.rowCount() == 0 );

  list.insertRows( 0, 1, Item() );

  CHECK( list.rowCount() == 1 );
}

TEST_CASE("ListWithAppend_example")
{
  ListWithAppendAdapted list;
  REQUIRE( list.rowCount() == 0 );

  list.appendRow( Item() );

  CHECK( list.rowCount() == 1 );
}

TEST_CASE("ListWithErase_example")
{
  ListWithEraseAdapted list( ListWithErase::fromItemList({{1,"A"}}) );
  REQUIRE( list.rowCount() == 1 );
  REQUIRE( list.atRow(0).name == "A" );

  list.removeRows(0, 1);

  CHECK( list.rowCount() == 0 );
}

TEST_CASE("maxRowCount_No_maxSize")
{
  ListWithInsertAdapted list;

  /*
   * We use std::vector in the test containers,
   * size_type is unsigned and at least unsigned int
   * (but probably an unsigned 64 bit int).
   */
  CHECK( list.maxRowCount() == intMax() );
}

TEST_CASE("maxRowCount_With_maxSize")
{
  ListWithInsertAndLimitAdapted list;

  list.containerMutable().setMaximumElementCount(25);

  CHECK( list.maxRowCount() == 25 );
}

TEST_CASE("canAddCountRows")
{
  ListWithInsertAndLimitAdapted list;
  list.containerMutable().setMaximumElementCount(5);
  REQUIRE( list.maxRowCount() == 5 );
  REQUIRE( list.rowCount() == 0 );

  SECTION("empty list")
  {
    CHECK( list.canAddCountRows(1) );
    CHECK( list.canAddCountRows(2) );
    CHECK( list.canAddCountRows(4) );
    CHECK( list.canAddCountRows(5) );
    CHECK( !list.canAddCountRows(6) );
    CHECK( !list.canAddCountRows(7) );
    CHECK( !list.canAddCountRows( intMax() ) );
  }

  SECTION("list with 2 elements")
  {
    list.insertRows( 0, 2, Item() );
    REQUIRE( list.rowCount() == 2 );

    CHECK( list.canAddCountRows(1) );
    CHECK( list.canAddCountRows(2) );
    CHECK( list.canAddCountRows(3) );
    CHECK( !list.canAddCountRows(4) );
    CHECK( !list.canAddCountRows(5) );
    CHECK( !list.canAddCountRows( intMax() ) );
  }
}

// TEMPLATE_TEST_CASE("default_constructed", "", TestContainerAdapter, SharedTestContainerAdapter)
// {
//   TestType container;
// 
//   CHECK( container.size() == 0 );
// }
