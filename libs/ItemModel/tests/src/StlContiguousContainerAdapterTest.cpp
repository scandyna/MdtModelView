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
#include <vector>

#include "Item.h"
#include "ReadOnlyList.h"
#include "DefaultConstructibleOnlyList.h"
#include "CopyConstructibleOnlyList.h"
#include "MoveConstructibleOnlyList.h"
#include "ReadOnlyListTableModelAdapterFunctionMap.h"
#include "DefaultConstructibleOnlyListTableModelAdapterFunctionMap.h"
#include "CopyConstructibleOnlyListTableModelAdapterFunctionMap.h"
#include "MoveConstructibleOnlyListTableModelAdapterFunctionMap.h"
#include "MutableList.h"
#include "MutableListRawDataTableModelAdapterFunctionMap.h"
#include "MutableList.h"
#include "MutableListTableModelAdapterFunctionMap.h"
#include "ListWithInsert.h"
#include "ListWithInsertTableModelAdapterFunctionMap.h"
#include "ListWithAppend.h"
#include "ListWithAppendTableModelAdapterFunctionMap.h"
#include "ListWithErase.h"
#include "ListWithEraseTableModelAdapterFunctionMap.h"

#include "Mdt/ItemModel/SharedStlContiguousContainerAdapter.h"

#include "Mdt/ItemModel/StlHelpers.h"

#include <functional>
#include <type_traits>

#include <QVariant>

using namespace Mdt::ItemModel;

// using TestContainerAdapter = StlContiguousContainerAdapter< std::vector<int> >;
// using SharedTestContainerAdapter = SharedStlContiguousContainerAdapter< std::vector<int> >;



  /*! \brief
   *
   * size() could be done with iterators
   *
   * at()   could be done with iterators
   *  -> Domain specific preconditions ?
   *
   * setAt() could be done with iterators
   *  -> Domain specific preconditions and logic ?
   *
   * \todo Some interface, STL, and user custom
   */
  template<typename Derived, typename Container>
  struct Xy_FunctionMap
  {

    /// \todo See about constructor: https://en.cppreference.com/w/cpp/language/crtp

    static
    size_t size(const Container & container)
    {
      return container.size();
      
      return Derived::sizeImpl();
    }

    static
    constexpr
    bool supportsInsert() noexcept
    {
      return true;
    }

    static
    void insert(Container & container)
    {
    }
    // using SizeFunction = typename Container::size;

  };

  /*! \brief
   *
   * \todo Maybe StlContiguousContainerTypeMap
   */
  template<typename Container>
  struct StlContiguousContainerTypeMap
  {
    /*! \brief STL size_type
     */
    using size_type = typename Container::size_type;

    /*! \brief STL difference_type
     */
    using difference_type = typename Container::difference_type;

    /*! \brief STL value_type
     */
    using value_type = typename Container::value_type;

    /*! \brief STL const_reference
     */
    using reference = typename Container::reference;

    /*! \brief STL const_reference
     *
     * \todo document that const_reference is deduced from value_type, or use const_reference
     */
    using const_reference = const value_type &;

    /*! \brief STL const_iterator
     */
    using const_iterator = typename Container::const_iterator;
  };

  /*! \brief STL read only ContiguousContainer function map for STL adapters
   *
   * This function map has the minimal requirements to provide read-only access.
   *
   * \todo brings no value.
   */
  template< typename Container>
  struct StlReadOnlyContiguousContainerFunctionMap
  {
    /*! \brief STL size_type
     */
    using size_type = typename Container::size_type;

  };

  /*! \brief STL ContiguousContainer function map for STL adapters
   *
   * \todo Can we relax to SequenceContainer ?
   * - https://en.cppreference.com/w/cpp/named_req/SequenceContainer
   * - https://en.cppreference.com/w/cpp/named_req/ContiguousContainer
   *
   * \todo Maybe TypeMap not required ?
   *
   * \todo Maybe inheritance like iterators ?
   *
   * \sa https://en.cppreference.com/w/cpp/named_req/ContiguousContainer
   */
  template< typename Container, typename TypeMap = StlContiguousContainerTypeMap<Container> >
  struct StlContiguousContainerFunctionMap
  {
    /*! \brief STL size_type
     */
    using size_type = typename TypeMap::size_type;

    /*! \brief STL difference_type
     */
    using difference_type = typename TypeMap::difference_type;

    /*! \brief STL const_reference
     */
    using const_reference = typename TypeMap::const_reference;

    /*! \brief STL const_iterator
     */
    using const_iterator = typename TypeMap::const_iterator;

    /*! \brief Check if const_iterator is provided
     *
     * Returns true,
     * because const_iterator, cbegin() and cend() are provided
     * by an STL conform ContiguousContainer .
     *
     * \todo remove
     */
    static
    constexpr
    bool providesConstIterator() noexcept
    {
      return true;
    }

    /*! \brief Check if insert() is supported
     *
     * Returns true
     */
    static
    constexpr
    bool supportsInsert() noexcept
    {
      return true;
    }

    /*! \brief Check if erase() is supported
     *
     * Returns true
     */
    static
    constexpr
    bool supportsErase() noexcept
    {
      return true;
    }

    /*! \brief
     */
    static
    const_iterator cbegin(const Container & container)
    {
      return container.cbegin();
    }

    /*! \brief insert function
     */
    static
    void insert(Container & container, const_iterator pos, size_type count, const_reference value)
    {
      container.insert(pos, count, value);
    }

    /*! \brief Erase function
     */
    static
    void erase(Container & container, const_iterator first, const_iterator last)
    {
      container.erase(first, last);
    }
  };

namespace Impl{

  // template<typename, typename, typename = void>
  // struct TypeMemberOr_void
  // {
  //   using type = void;
  // };
  // 
  // template<typename Container, typename Type>
  // struct TypeMemberOr_void< Container, Type, std::void_t<typename Container::Type> >
  // {
  //   using type = typename Container::Type;
  // };


  template<typename, typename = void>
  struct difference_type_TypeMemberOr_void
  {
    using type = void;
  };

  template<typename T>
  struct difference_type_TypeMemberOr_void< T, std::void_t<typename T::difference_type> >
  {
    using type = typename T::difference_type;
  };


  template<typename, typename = void>
  struct reference_TypeMemberOr_void
  {
    using type = void;
  };

  template<typename T>
  struct reference_TypeMemberOr_void< T, std::void_t<typename T::reference> >
  {
    using type = typename T::reference;
  };


  template<typename, typename = void>
  struct const_iterator_TypeMemberOr_void
  {
    using type = void;
  };

  template<typename T>
  struct const_iterator_TypeMemberOr_void< T, std::void_t<typename T::const_iterator> >
  {
    using type = typename T::const_iterator;
  };

} // namespace Impl{


struct MyFunctionMap
{
};

struct MyFunctionMapWithReference
{
  using reference = int &;
};

template<typename FunctionMap>
struct Adapter
{
  using reference = typename Impl::reference_TypeMemberOr_void<FunctionMap>::type;

  reference value() const
  {
  }
};


struct MyModel
{
  Adapter<MyFunctionMap> a;
};

struct MyModelWithReference
{
  Adapter<MyFunctionMapWithReference> a;

  int & value() const
  {
    return a.value();
  }
};


/**
 * \sa https://en.cppreference.com/w/cpp/types/void_t
 * \sa https://en.cppreference.com/w/cpp/types/integral_constant
 */
// primary template handles types that have no nested ::type member:
// template<typename, typename = void>
// struct has_type_member : std::false_type {};
 
// specialization recognizes types that do have a nested ::type member:
// template<typename T>
// struct has_type_member<T, std::void_t<typename T::type>> : std::true_type {};


  /*! \brief Adapter to use STL style containers with Qt item models
   *
   * In Qt model/view, row acces is int index based.
   * In the STL containers, index is std::size_t or iterator based.
   *
   * A goal of this adapter is to help to convert between those types
   * in some checked way (contract programming based).
   *
   * This adapter can be used with containers that are domain specific,
   * and that do not provide all the STL required interface.
   *
   * Goal is to avoid having to adapt the container itself
   * to be usable with an item model.
   *
   * \todo fix noexcept
   *
   * \tparam Container
   * \tparam SizeType
   *
   * \tparam ValueType STL value_type provided by the container.
   * This is a required type, void is not allowed.
   * const_reference is also deduced from ValueType.
   *
   * \tparam ConstIterator STL const_iterator provided by the container.
   * Can be void if the container not provides const_iterator.
   * If the container provides const_iterator,
   * it also must provide cbegin() and cend().
   *
   * \tparam Reference STL reference provided by the container.
   * 
   *
   * \tparam FunctionMap
   *
   * \sa Mdt::ItemModel::AbstractTableModel
   * \sa https://doc.qt.io/qt-6/qabstractitemmodel.html
   * \sa https://doc.qt.io/qt-6/qmodelindex.html
   */
  template<typename Container, typename FunctionMap>
  struct StlContainerAdapter
  {
    // static_assert( !std::is_void_v<typename TypeMap::value_type> );

    /*! \brief STL size_type
     */
    using size_type = typename FunctionMap::size_type;

    /*! \brief STL difference_type
     *
     * Will be FunctionMap::difference_type if \a FunctionMap defines it,
     * otherwise void.
     */
    using difference_type = typename Impl::difference_type_TypeMemberOr_void<FunctionMap>::type;

    /*! \brief STL reference
     *
     * Will be FunctionMap::reference if \a FunctionMap defines it,
     * otherwise void.
     */
    using reference = typename Impl::reference_TypeMemberOr_void<FunctionMap>::type;
    // using reference = typename Impl::TypeMemberOr_void<FunctionMap, typename FunctionMap::reference>::type;
    // using reference = typename TypeMap::reference;

    /*! \brief STL const_reference
     */
    using const_reference = typename FunctionMap::const_reference;

    /*! \brief STL const_iterator
     *
     * Will be FunctionMap::const_iterator if \a FunctionMap defines it,
     * otherwise void.
     */
    using const_iterator = typename Impl::const_iterator_TypeMemberOr_void<FunctionMap>::type;
    // using const_iterator = typename TypeMap::const_iterator;

    // using const_reference = typename Container::const_reference;

    /*! \brief Get the count of rows for the model
     *
     * \pre The current size of the container must be convertible to int
     */
    int rowCount() const
    {
      /// \todo use size_type
      return FunctionMap::size(mContainer);
      // return std::invoke(FunctionMap::GetSize, mContainer);
      // return std::invoke(FunctionMap::SizeFunction, mContainer);
    }

    /*! \brief Get the element at given row
     *
     * \pre \a row must be in range ( 0 >= \a row < rowCount() )
     */
    const_reference atRow(int row) const noexcept
    {
      /// \todo handle to size_type conversion
      return FunctionMap::atIndex(mContainer, row);
    }

    /*! \brief Access the element at given row for mutation
     *
     * \pre the reference type must be valid.
     * \pre \a row must be in range ( 0 >= \a row < rowCount() )
     */
    // template<typename ReferenceType>
    reference atRowMutable(int row) noexcept
    {
      static_assert( !std::is_void_v<reference>, "call StlContiguousContainerAdapter::atRowMutable() requires FunctionMap::reference to be defined" );

      /// \todo use size_type
      
      return mContainer[row];
    }

    /*! \brief Access the container
     *
     * Should only be used when required,
     * like for find row functions.
     */
    const Container & container() const noexcept
    {
      return mContainer;
    }

    /*! \brief Get the row corresponding to given position
     *
     * \pre \a pos must be of type of the container's const_iterator
     *
     * \todo use const_iterator
     */
    template<typename Iterator>
    int rowFromPosition(Iterator pos) const
    {
      // static_assert( std::is_same_v<Iterator, const_iterator> );
    }

    // int rowFromPosition(typename std::enable_if_t<!std::is_void_v<const_iterator>, const_iterator>::type  pos) const
    // {
    // }

    /*! \brief Get the row for given index
     */
    int rowFromIndex(size_type index) const
    {
    }

    /// get data

    /*! \brief Inserts count rows into the container before the given row
     *
     * \todo preconditions
     */
    bool insertRows(int row, int count, const_reference value)
    {
      if constexpr( FunctionMap::supportsInsert() ){
        static_assert( !std::is_void_v<difference_type>, "call StlContiguousContainerAdapter::insertRows() requires FunctionMap::difference_type to be defined" );
        static_assert( !std::is_void_v<const_iterator>, "call StlContiguousContainerAdapter::insertRows() requires FunctionMap::const_iterator to be defined" );

        /// calc iterator + difference + check + cast
        // const auto pos = const_iterator{};
        /// \todo adapt and use insertToStlContainer()
        // FunctionMap::insert(mContainer);
        // return true;
      }
      return false;
    }

    /*! \brief Removes count rows starting with the given row
     *
     * \todo preconditions
     */
    bool removeRows(int row, int count)
    {
      if constexpr( FunctionMap::supportsErase() ){
        static_assert( !std::is_void_v<difference_type>, "call StlContiguousContainerAdapter::removeRows() requires FunctionMap::difference_type to be defined" );
        static_assert( !std::is_void_v<const_iterator>, "call StlContiguousContainerAdapter::removeRows() requires FunctionMap::const_iterator to be defined" );

        /// \todo Adapt and use removeFromStlContainer()
      }
      return false;
    }

    Container mContainer;
  };



  /** List that provides insert
   */



  /** List that provides append
   */


  /** Read only and resizable example
   *
   */

  struct ReadOnlyResizableList
  {
    using size_type = std::vector<Item>::size_type;
    using const_iterator = std::vector<Item>::const_iterator;

    size_type getSizeCustom() const noexcept
    {
      return 25;
    }

    const Item & itemAt(size_type index) const noexcept
    {
    }

    void insert(const_iterator pos, size_type count, const Item & value)
    {
    }

    void erase(const_iterator first, const_iterator last)
    {
    }
  };

  struct ReadOnlyResizableListTableModelAdapterFunctionMap
  {
    using size_type = ReadOnlyResizableList::size_type;
    using const_reference = const Item &;
    using const_iterator = ReadOnlyResizableList::const_iterator;

    static
    constexpr
    bool supportsInsert() noexcept
    {
      return true;
    }

    static
    constexpr
    bool supportsErase() noexcept
    {
      return true;
    }

    static
    size_type size(const ReadOnlyResizableList & list) noexcept
    {
      return list.getSizeCustom();
    }

    static
    const_reference atIndex(const ReadOnlyResizableList & list, size_type index) noexcept
    {
      return list.itemAt(index);
    }

    /// \todo Very common: only support push_back

    static
    void insert(ReadOnlyResizableList & list, const_iterator pos, size_type count, const_reference value)
    {
      list.insert(pos, count, value);
    }

    /*! \brief Erase function
     */
    static
    void erase(ReadOnlyResizableList & list, const_iterator first, const_iterator last)
    {
      list.erase(first, last);
    }
  };

  struct MyReadOnlyResizableListTableModel
  {
    int rowCount() const
    {
      return mList.rowCount();
    }
  
//     int findRowOfId(int id) const noexcept
//     {
//       const auto it = mList.container().findItemWithId(id);
//       return mList.rowFromPosition(it);
//   
//       // const auto pred = [id](const MyItem & item) -> bool {
//       //   return MyList::isRequestedItem(item, id);
//       // };
//       // return mList.findRowOf(pred);
//     }
  
    bool insertRows(int row, int count)
    {
      // beginInsertRows() omitted
      /// return mList.insertRows( row, count, Item() );
      // endInsertRows() omitted
    }
  
    // MyItem & sandboxMutableData(int row)
    // {
    //   // return mList.atRowMutable(row);
    // }
  
    // bool insert()
    // {
    //   return mList.insert();
    // }
  
    StlContainerAdapter<ReadOnlyResizableList, ReadOnlyResizableListTableModelAdapterFunctionMap> mList;
  };


  /** Mutable and resizable example
   */

  struct MutableResizableList
  {
    using size_type = std::vector<Item>::size_type;
    using const_iterator = std::vector<Item>::const_iterator;

    size_type getSizeCustom() const noexcept
    {
      return 25;
    }

    const Item & itemAt(size_type index) const noexcept
    {
    }

    Item & mutableItemAt(size_type index) noexcept
    {
    }

    void insert(const_iterator pos, size_type count, const Item & value)
    {
    }

    void erase(const_iterator first, const_iterator last)
    {
    }
  };

  struct MutableResizableListTableModelAdapterFunctionMap
  {
    using size_type = MutableResizableList::size_type;
    using reference = Item &;
    using const_reference = const Item &;
    using const_iterator = MutableResizableList::const_iterator;

    static
    constexpr
    bool supportsInsert() noexcept
    {
      return true;
    }

    static
    constexpr
    bool supportsErase() noexcept
    {
      return true;
    }

    static
    size_type size(const MutableResizableList & list) noexcept
    {
      return list.getSizeCustom();
    }

    static
    const_reference atIndex(const MutableResizableList & list, size_type index) noexcept
    {
      return list.itemAt(index);
    }

    static
    reference atIndexMutable(MutableResizableList & list, size_type index) noexcept
    {
      return list.mutableItemAt(index);
    }

    static
    void insert(MutableResizableList & list, const_iterator pos, size_type count, const_reference value)
    {
      list.insert(pos, count, value);
    }

    /*! \brief Erase function
     */
    static
    void erase(MutableResizableList & list, const_iterator first, const_iterator last)
    {
      list.erase(first, last);
    }
  };

  /// \todo table model

  /** Read only example with iterator based find
   */

  struct MyReadOnlyWithIteratorFindList
  {
    using size_type = std::vector<Item>::size_type;
    using const_iterator = std::vector<Item>::const_iterator;

    size_type getSizeCustom() const noexcept
    {
      return 25;
    }

    const Item & itemAt(size_type index) const noexcept
    {
    }

    const_iterator findItemWithId(int id) const noexcept
    {
    }
  };

  struct MyReadOnlyWithIteratorFindListTableModelAdapterFunctionMap
  {
    using size_type = MyReadOnlyWithIteratorFindList::size_type;
    using const_reference = const Item &;
    using const_iterator = MyReadOnlyWithIteratorFindList::const_iterator;

    static
    constexpr
    bool supportsInsert() noexcept
    {
      return false;
    }

    static
    constexpr
    bool supportsErase() noexcept
    {
      return false;
    }

    static
    size_type size(const MyReadOnlyWithIteratorFindList & list) noexcept
    {
      return list.getSizeCustom();
    }

    static
    const_reference atIndex(const MyReadOnlyWithIteratorFindList & list, size_type index) noexcept
    {
      return list.itemAt(index);
    }
  };

  /// \todo table model

  /** Read only example with index based find
   */

  struct MyReadOnlyWithIndexFindList
  {
    using size_type = std::vector<Item>::size_type;

    size_type getSizeCustom() const noexcept
    {
      return 25;
    }

    const Item & itemAt(size_type index) const noexcept
    {
    }

    size_type findIndexOfItemWithId(int id) const noexcept
    {
    }
  };

  struct MyReadOnlyWithIndexFindListTableModelAdapterFunctionMap
  {
    using size_type = MyReadOnlyWithIndexFindList::size_type;
    using const_reference = const Item &;

    static
    constexpr
    bool supportsInsert() noexcept
    {
      return false;
    }

    static
    constexpr
    bool supportsErase() noexcept
    {
      return false;
    }

    static
    size_type size(const MyReadOnlyWithIndexFindList & list) noexcept
    {
      return list.getSizeCustom();
    }

    static
    const_reference atIndex(const MyReadOnlyWithIndexFindList & list, size_type index) noexcept
    {
      return list.itemAt(index);
    }
  };

  /// \todo table model


  /** Read only example
   *
   */


  /**
   * NOTE: for adapter
   *
   * - size() and atIndex() always required
   * - size_type and const_reference always required
   *
   * - size_type has to be exposed by the container
   *
   * \todo Maybe create a CRTP based interface that forces implementing supportsInsert() etc.. ?
   */


  struct ReadOnlyListTableModel
  {
    ReadOnlyListTableModel(const ReadOnlyList & list)
    : mList(list)
    {
    }

    int rowCount() const
    {
      return mList.rowCount();
    }

    QVariant data(int row, int column) const
    {
      switch(column){
        // case 0:
        //   return mList.atRow(row).id;
        // case 1:
        //   return mList.atRow(row).name;
      }
      return QVariant();
    }

    StlContiguousContainerAdapter<ReadOnlyList, ReadOnlyListTableModelAdapterFunctionMap> mList;
  };


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

/*
 * Types for constructors tests
 */
using DefaultConstructibleOnlyListAdapted = StlContiguousContainerAdapter<DefaultConstructibleOnlyList, DefaultConstructibleOnlyListTableModelAdapterFunctionMap>;
using CopyConstructibleOnlyListAdapted = StlContiguousContainerAdapter<CopyConstructibleOnlyList, CopyConstructibleOnlyListTableModelAdapterFunctionMap>; 
using MoveConstructibleOnlyListAdapted = StlContiguousContainerAdapter<MoveConstructibleOnlyList, MoveConstructibleOnlyListTableModelAdapterFunctionMap>;

using ReadOnlyListContainerAndFunctionMap = ContainerAndFunctionMap<ReadOnlyList, ReadOnlyListTableModelAdapterFunctionMap>;
using ReadOnlyListAdapted = StlContiguousContainerAdapter<ReadOnlyList, ReadOnlyListTableModelAdapterFunctionMap>;


  /** Mutable example (NOT resizable)
   */

using MutableListRawDataAdapted = StlContiguousContainerAdapter<MutableListRawData, MutableListRawDataTableModelAdapterFunctionMap>;
using MutableListAdapted = StlContiguousContainerAdapter<MutableList, MutableListTableModelAdapterFunctionMap>;

using ListWithInsertAdapted = StlContiguousContainerAdapter<ListWithInsert, ListWithInsertTableModelAdapterFunctionMap>;
using ListWithAppendAdapted = StlContiguousContainerAdapter<ListWithAppend, ListWithAppendTableModelAdapterFunctionMap>;

using ListWithEraseAdapted = StlContiguousContainerAdapter<ListWithErase, ListWithEraseTableModelAdapterFunctionMap>;

  /// \todo table model

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

TEST_CASE("rowFromPosition")
{
  /// REQUIRE(false);
}

TEST_CASE("rowFromIndex")
{
  /// REQUIRE(false);
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

  REQUIRE( list.insertRows( 0, 1, Item() ) );

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

// TEMPLATE_TEST_CASE("default_constructed", "", TestContainerAdapter, SharedTestContainerAdapter)
// {
//   TestType container;
// 
//   CHECK( container.size() == 0 );
// }
