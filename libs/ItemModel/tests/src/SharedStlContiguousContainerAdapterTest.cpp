// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "Mdt/ItemModel/SharedStlContiguousContainerAdapter.h"
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "Item.h"
#include "DefaultConstructibleOnlyList.h"
#include "CopyConstructibleOnlyList.h"
#include "MoveConstructibleOnlyList.h"
#include "DefaultConstructibleOnlyListTableModelAdapterFunctionMap.h"
#include "CopyConstructibleOnlyListTableModelAdapterFunctionMap.h"
#include "MoveConstructibleOnlyListTableModelAdapterFunctionMap.h"
#include "ReadOnlyList.h"
#include "ReadOnlyListTableModelAdapterFunctionMap.h"
#include "MutableListRawData.h"
#include "MutableListRawDataTableModelAdapterFunctionMap.h"
#include "MutableList.h"
#include "MutableListTableModelAdapterFunctionMap.h"
#include "ListWithInsert.h"
#include "ListWithInsertTableModelAdapterFunctionMap.h"
#include "ListWithInsertAndLimit.h"
#include "ListWithInsertAndLimitTableModelAdapterFunctionMap.h"
#include "ListWithAppend.h"
#include "ListWithAppendTableModelAdapterFunctionMap.h"
#include "ListWithErase.h"
#include "ListWithEraseTableModelAdapterFunctionMap.h"
#include "ReadOnlyWithIteratorFindList.h"
#include "ReadOnlyWithIteratorFindListTableModelAdapterFunctionMap.h"

using namespace Mdt::ItemModel;


using DefaultConstructibleOnlyListAdapted = SharedStlContiguousContainerAdapter<DefaultConstructibleOnlyList, DefaultConstructibleOnlyListTableModelAdapterFunctionMap>;
using CopyConstructibleOnlyListAdapted = SharedStlContiguousContainerAdapter<CopyConstructibleOnlyList, CopyConstructibleOnlyListTableModelAdapterFunctionMap>;
using MoveConstructibleOnlyListAdapted = SharedStlContiguousContainerAdapter<MoveConstructibleOnlyList, MoveConstructibleOnlyListTableModelAdapterFunctionMap>;

using ReadOnlyListAdapted = SharedStlContiguousContainerAdapter<ReadOnlyList, ReadOnlyListTableModelAdapterFunctionMap>;

using MutableListRawDataAdapted = SharedStlContiguousContainerAdapter<MutableListRawData, MutableListRawDataTableModelAdapterFunctionMap>;
using MutableListAdapted = SharedStlContiguousContainerAdapter<MutableList, MutableListTableModelAdapterFunctionMap>;

using ListWithInsertAdapted = SharedStlContiguousContainerAdapter<ListWithInsert, ListWithInsertTableModelAdapterFunctionMap>;
using ListWithAppendAdapted = SharedStlContiguousContainerAdapter<ListWithAppend, ListWithAppendTableModelAdapterFunctionMap>;
using ListWithInsertAndLimitAdapted = SharedStlContiguousContainerAdapter<ListWithInsertAndLimit, ListWithInsertAndLimitTableModelAdapterFunctionMap>;

using ListWithEraseAdapted = SharedStlContiguousContainerAdapter<ListWithErase, ListWithEraseTableModelAdapterFunctionMap>;

using ReadOnlyWithIteratorFindListAdapted = SharedStlContiguousContainerAdapter<ReadOnlyWithIteratorFindList, ReadOnlyWithIteratorFindListTableModelAdapterFunctionMap>;


TEMPLATE_TEST_CASE("default_constructed", "", DefaultConstructibleOnlyListAdapted, CopyConstructibleOnlyListAdapted, MoveConstructibleOnlyListAdapted, ReadOnlyListAdapted)
{
  TestType list;

  CHECK( list.rowCount() == 0 );
}


TEST_CASE("setContainer")
{
  ReadOnlyListAdapted adapter;

  CHECK( adapter.rowCount() == 0 );

  auto list = std::make_shared<ReadOnlyList>( ReadOnlyList::fromItemList({{1,"A"}}) );

  adapter.setContainer(list);

  CHECK( adapter.rowCount() == 1 );
}

TEST_CASE("maxRowCount_No_maxSize")
{
  ListWithInsertAdapted adapter;

  CHECK( adapter.maxRowCount() == 0 );

  auto list = std::make_shared<ListWithInsert>();

  adapter.setContainer(list);

  /*
   * We use std::vector in the test containers,
   * size_type is unsigned and at least unsigned int
   * (but probably an unsigned 64 bit int).
   */
  CHECK( adapter.maxRowCount() == intMax() );
}

TEST_CASE("maxRowCount_With_maxSize")
{
  ListWithInsertAndLimitAdapted adapter;

  CHECK( adapter.maxRowCount() == 0 );

  auto list = std::make_shared<ListWithInsertAndLimit>();
  list->setMaximumElementCount(25);

  adapter.setContainer(list);

  CHECK( adapter.maxRowCount() == 25 );
}

TEST_CASE("canAddCountRows")
{
  ListWithInsertAndLimitAdapted adapter;
  auto list = std::make_shared<ListWithInsertAndLimit>();
  list->setMaximumElementCount(5);
  adapter.setContainer(list);
  REQUIRE( adapter.maxRowCount() == 5 );
  REQUIRE( adapter.rowCount() == 0 );

  SECTION("empty list")
  {
    CHECK( adapter.canAddCountRows(1) );
    CHECK( adapter.canAddCountRows(2) );
    CHECK( adapter.canAddCountRows(4) );
    CHECK( adapter.canAddCountRows(5) );
    CHECK( !adapter.canAddCountRows(6) );
    CHECK( !adapter.canAddCountRows(7) );
    CHECK( !adapter.canAddCountRows( intMax() ) );
  }

  SECTION("adapter with 2 elements")
  {
    adapter.insertRows( 0, 2, Item() );
    REQUIRE( adapter.rowCount() == 2 );

    CHECK( adapter.canAddCountRows(1) );
    CHECK( adapter.canAddCountRows(2) );
    CHECK( adapter.canAddCountRows(3) );
    CHECK( !adapter.canAddCountRows(4) );
    CHECK( !adapter.canAddCountRows(5) );
    CHECK( !adapter.canAddCountRows( intMax() ) );
  }
}

TEST_CASE("indexFromRow")
{
  ReadOnlyListAdapted adapter;
  auto list = std::make_shared<ReadOnlyList>( ReadOnlyList::fromItemList({{1,"A"}}) );
  adapter.setContainer(list);
  REQUIRE( adapter.rowCount() == 1 );

  CHECK( adapter.indexFromRow(0) == 0 );
}

TEST_CASE("rowFromIndex")
{
  ReadOnlyListAdapted adapter;
  auto list = std::make_shared<ReadOnlyList>( ReadOnlyList::fromItemList({{1,"A"},{2,"B"}}) );
  adapter.setContainer(list);
  REQUIRE( adapter.rowCount() == 2 );

  CHECK( adapter.rowFromIndex(0) == 0 );
  CHECK( adapter.rowFromIndex(1) == 1 );
}

TEST_CASE("positionIsInRange")
{
  ReadOnlyWithIteratorFindListAdapted adapter;
  auto list = std::make_shared<ReadOnlyWithIteratorFindList>( ReadOnlyWithIteratorFindList::fromItemList({{1,"A"},{2,"B"},{3,"C"}}) );
  adapter.setContainer(list);
  REQUIRE( adapter.rowCount() == 3 );

  CHECK( adapter.positionIsInRange( list->cbegin() ) );
  CHECK( adapter.positionIsInRange( list->cbegin()+1 ) );
  CHECK( adapter.positionIsInRange( list->cbegin()+2 ) );
  CHECK( !adapter.positionIsInRange( list->cbegin()+3 ) );
  CHECK( !adapter.positionIsInRange( list->cend() ) );
}

TEST_CASE("rowFromPosition")
{
  ReadOnlyWithIteratorFindListAdapted adapter;
  auto list = std::make_shared<ReadOnlyWithIteratorFindList>( ReadOnlyWithIteratorFindList::fromItemList({{1,"A"},{2,"B"},{3,"C"}}) );
  adapter.setContainer(list);
  REQUIRE( adapter.rowCount() == 3 );

  CHECK( adapter.rowFromPosition( list->cbegin() ) == 0 );
  CHECK( adapter.rowFromPosition( list->cbegin()+1 ) == 1 );
  CHECK( adapter.rowFromPosition( list->cbegin()+2 ) == 2 );
}

TEST_CASE("ReadOnly_example")
{
  ReadOnlyListAdapted adapter;
  auto list = std::make_shared<ReadOnlyList>( ReadOnlyList::fromItemList({{1,"A"}}) );
  adapter.setContainer(list);

  CHECK( adapter.rowCount() == 1 );
  CHECK( adapter.atRow(0).id == 1 );
}

TEST_CASE("MutableListRawData_example")
{
  MutableListRawDataAdapted adapter;
  auto list = std::make_shared<MutableListRawData>( MutableListRawData::fromItemList({{1,"A"}}) );
  adapter.setContainer(list);
  REQUIRE( adapter.rowCount() == 1 );
  REQUIRE( adapter.atRow(0).name == "A" );

  adapter.atRowMutable(0).name = "B";

  CHECK( adapter.atRow(0).name == "B" );
}

TEST_CASE("MutableList_DomainMethods_example")
{
  MutableListAdapted adapter;
  auto list = std::make_shared<MutableList>( MutableList::fromItemList({{1,"A"}}) );
  adapter.setContainer(list);
  REQUIRE( adapter.rowCount() == 1 );
  REQUIRE( adapter.atRow(0).name == "A" );

  const MutableList::size_type containerIndex = adapter.indexFromRow(0);
  adapter.containerMutable().setNameAt(containerIndex, "C");

  CHECK( adapter.atRow(0).name == "C" );

  adapter.containerMutablePtr()->setNameAt(containerIndex, "D");

  CHECK( adapter.atRow(0).name == "D" );

  list->setNameAt(containerIndex, "E");

  CHECK( adapter.atRow(0).name == "E" );
}

TEST_CASE("ListWithInsert_example")
{
  ListWithInsertAdapted adapter;
  auto list = std::make_shared<ListWithInsert>();
  adapter.setContainer(list);
  REQUIRE( adapter.rowCount() == 0 );

  adapter.insertRows( 0, 1, Item() );

  CHECK( adapter.rowCount() == 1 );
}

TEST_CASE("ListWithAppend_example")
{
  ListWithAppendAdapted adapter;
  auto list = std::make_shared<ListWithAppend>();
  adapter.setContainer(list);
  REQUIRE( adapter.rowCount() == 0 );

  adapter.appendRow( Item() );

  CHECK( adapter.rowCount() == 1 );
}

TEST_CASE("ListWithErase_example")
{
  ListWithEraseAdapted adapter;
  auto list = std::make_shared<ListWithErase>( ListWithErase::fromItemList({{1,"A"}}) );
  adapter.setContainer(list);
  REQUIRE( adapter.rowCount() == 1 );
  REQUIRE( adapter.atRow(0).name == "A" );

  adapter.removeRows(0, 1);

  CHECK( adapter.rowCount() == 0 );
}
