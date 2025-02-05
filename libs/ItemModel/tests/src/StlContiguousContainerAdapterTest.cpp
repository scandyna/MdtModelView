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

#include "Mdt/ItemModel/SharedStlContiguousContainerAdapter.h"

#include "Mdt/ItemModel/StlHelpers.h"

#include <functional>
#include <type_traits>

using namespace Mdt::ItemModel;

using TestContainerAdapter = StlContiguousContainerAdapter< std::vector<int> >;
using SharedTestContainerAdapter = SharedStlContiguousContainerAdapter< std::vector<int> >;



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

  /*! \brief Adapter to use STL style containers with Qt item models
   *
   * In Qt model/view, row acces is int index based.
   * In the STL containers, index is std::size_t or iterator based.
   *
   * A goal of this adapter is to help to convert between those types
   * in some checked way (contract programming based).
   *
   * This adapter can be used containers that are domain specific,
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
  template<
    typename Container,
    typename TypeMap = StlContiguousContainerTypeMap<Container>, /// Maybe redoundant ??
    typename FunctionMap = StlContiguousContainerFunctionMap<Container>
  >
  struct StlContainerAdapter
  {
    
    static_assert( !std::is_void_v<typename TypeMap::value_type> );

    /*! \brief STL size_type
     */
    using size_type = typename TypeMap::size_type;

    /*! \brief STL const_reference
     */
    using reference = typename TypeMap::reference;

    /*! \brief STL const_reference
     */
    using const_reference = const typename TypeMap::value_type &;

    /*! \brief STL const_iterator
     */
    using const_iterator = typename TypeMap::const_iterator;

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
      /// \todo use size_type
    }

    /*! \brief Access the element at given row for mutation
     *
     * \pre the reference type must be valid.
     * \pre \a row must be in range ( 0 >= \a row < rowCount() )
     */
    reference atRowMutable(int row) noexcept
    {
      static_assert( !std::is_void_v<reference> );

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
     */
    template<typename Iterator>
    int rowFromPosition(Iterator pos) const
    {
      static_assert( std::is_same_v<Iterator, const_iterator> );
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
        /// calc iterator + difference + check + cast
        const auto pos = const_iterator{};
        /// \todo adapt and use insertToStlContainer()
        FunctionMap::insert(mContainer);
        return true;
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
        /// \todo Adapt and use removeFromStlContainer()
      }
      return false;
    }

    Container mContainer;
  };


  struct MyItem
  {
    int id = 0;
    QString name;
  };

  /** Read only example
   *
   */

  struct MyReadOnlyList
  {
    using size_type = std::vector<MyItem>::size_type;

    size_type getSizeCustom() const noexcept
    {
      return 25;
    }

    const MyItem & itemAt(size_type index) const noexcept
    {
    }
  };

  /**
   * NOTE:
   * - size() and atIndex() always required
   * - size_type and const_reference always required
   *
   * - size_type has to be exposed by the container
   *
   * \todo Maybe create a CRTP based interface that forces implementing supportsInsert() etc.. ?
   */
  struct MyReadOnlyListTableModelAdapterFunctionMap
  {
    using size_type = MyReadOnlyList::size_type;
    using const_reference = const MyItem &;

    static
    constexpr
    bool supportsAtIndexMutable() noexcept
    {
      return false;
    }

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
    size_type size(const MyReadOnlyList & list) noexcept
    {
      return list.getSizeCustom();
    }

    static
    const_reference atIndex(const MyReadOnlyList & list, size_type index) noexcept
    {
      return list.itemAt(index);
    }
  };


  /** Mutable example (NOT resizable)
   */

  struct MyMutableList
  {
    using size_type = std::vector<MyItem>::size_type;

    size_type getSizeCustom() const noexcept
    {
      return 25;
    }

    const MyItem & itemAt(size_type index) const noexcept
    {
    }

    MyItem & mutableItemAt(size_type index) noexcept
    {
    }
  };

  struct MyMutableListFunctionMap
  {
    using size_type = MyMutableList::size_type;
    using const_reference = const MyItem &;
    using reference = MyItem &;

    static
    constexpr
    bool supportsAtIndexMutable() noexcept
    {
      return true;
    }

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
    size_type size(const MyMutableList & list) noexcept
    {
      return list.getSizeCustom();
    }

    static
    const_reference atIndex(const MyMutableList & list, size_type index) noexcept
    {
      return list.itemAt(index);
    }

    static
    reference atIndexMutable(MyMutableList & list, size_type index) noexcept
    {
      return list.mutableItemAt(index);
    }
  };


  /** Read only and resizable example
   *
   */

  struct MyReadOnlyResizableList
  {
    using size_type = std::vector<MyItem>::size_type;
    using const_iterator = std::vector<MyItem>::const_iterator;

    size_type getSizeCustom() const noexcept
    {
      return 25;
    }

    const MyItem & itemAt(size_type index) const noexcept
    {
    }

    void insert(const_iterator pos, size_type count, const MyItem & value)
    {
    }

    void erase(const_iterator first, const_iterator last)
    {
    }
  };

  struct MyReadOnlyResizableListFunctionMap
  {
    using size_type = MyReadOnlyResizableList::size_type;
    using const_reference = const MyItem &;
    using const_iterator = MyReadOnlyResizableList::const_iterator;

    static
    constexpr
    bool supportsAtIndexMutable() noexcept
    {
      return false;
    }

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
    size_type size(const MyReadOnlyResizableList & list) noexcept
    {
      return list.getSizeCustom();
    }

    static
    const_reference atIndex(const MyReadOnlyResizableList & list, size_type index) noexcept
    {
      return list.itemAt(index);
    }

    /// \todo Very common: only support push_back

    static
    void insert(MyReadOnlyResizableList & list, const_iterator pos, size_type count, const_reference value)
    {
      list.insert(pos, count, value);
    }

    /*! \brief Erase function
     */
    static
    void erase(MyReadOnlyResizableList & list, const_iterator first, const_iterator last)
    {
      list.erase(first, last);
    }
  };


  /** Mutable and resizable example
   */

  struct MyMutableResizableList
  {
    using size_type = std::vector<MyItem>::size_type;
    using const_iterator = std::vector<MyItem>::const_iterator;

    size_type getSizeCustom() const noexcept
    {
      return 25;
    }

    const MyItem & itemAt(size_type index) const noexcept
    {
    }

    MyItem & mutableItemAt(size_type index) noexcept
    {
    }

    void insert(const_iterator pos, size_type count, const MyItem & value)
    {
    }

    void erase(const_iterator first, const_iterator last)
    {
    }
  };

  struct MyMutableResizableListTableModelAdapterFunctionMap
  {
    using size_type = MyMutableResizableList::size_type;
    using reference = MyItem &;
    using const_reference = const MyItem &;
    using const_iterator = MyMutableResizableList::const_iterator;

    static
    constexpr
    bool supportsAtIndexMutable() noexcept
    {
      return true;
    }

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
    size_type size(const MyMutableResizableList & list) noexcept
    {
      return list.getSizeCustom();
    }

    static
    const_reference atIndex(const MyMutableResizableList & list, size_type index) noexcept
    {
      return list.itemAt(index);
    }

    static
    reference atIndexMutable(MyMutableResizableList & list, size_type index) noexcept
    {
      return list.mutableItemAt(index);
    }

    static
    void insert(MyMutableResizableList & list, const_iterator pos, size_type count, const_reference value)
    {
      list.insert(pos, count, value);
    }

    /*! \brief Erase function
     */
    static
    void erase(MyMutableResizableList & list, const_iterator first, const_iterator last)
    {
      list.erase(first, last);
    }
  };


  /** Read only example with iterator based find
   */

  struct MyReadOnlyWithIteratorFindList
  {
    using size_type = std::vector<MyItem>::size_type;
    using const_iterator = std::vector<MyItem>::const_iterator;

    size_type getSizeCustom() const noexcept
    {
      return 25;
    }

    const MyItem & itemAt(size_type index) const noexcept
    {
    }

    const_iterator findItemWithId(int id) const noexcept
    {
    }
  };

  struct MyReadOnlyWithIteratorFindListTableModelAdapterFunctionMap
  {
    using size_type = MyReadOnlyWithIteratorFindList::size_type;
    using const_reference = const MyItem &;
    using const_iterator = MyReadOnlyWithIteratorFindList::const_iterator;

    static
    constexpr
    bool supportsAtIndexMutable() noexcept
    {
      return false;
    }

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


  /** Read only example with index based find
   */

  struct MyReadOnlyWithIndexFindList
  {
    using size_type = std::vector<MyItem>::size_type;

    size_type getSizeCustom() const noexcept
    {
      return 25;
    }

    const MyItem & itemAt(size_type index) const noexcept
    {
    }

    size_type findIndexOfItemWithId(int id) const noexcept
    {
    }
  };

  struct MyReadOnlyWithIndexFindListTableModelAdapterFunctionMap
  {
    using size_type = MyReadOnlyWithIndexFindList::size_type;
    using const_reference = const MyItem &;

    static
    constexpr
    bool supportsAtIndexMutable() noexcept
    {
      return false;
    }

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



  struct MyList
  {
    using const_iterator = std::vector<MyItem>::const_iterator;

    size_t getSizeCustom() const noexcept
    {
      return 25;
    }

    template<typename UnaryPred>
    const_iterator findItem(UnaryPred pred) const noexcept
    {
    }

    const_iterator findItemWithId(int id) const noexcept
    {
      const auto pred = [id](const MyItem & item){
        return isRequestedItem(item, id);
      };
      return findItem(pred);
    }

    static
    bool isRequestedItem(const MyItem & item, int id) noexcept
    {
    }
    
    std::vector<MyItem> mList;
  };

  // using MyListFunctionMap = Xy_FunctionMap<MyList>;

  struct MyListTypeMap
  {
    using size_type = size_t;
    using value_type = MyItem;
    using reference = void;
    // using const_reference = const value_type &;
    using const_iterator = void;
  };

  struct MyListFunctionMap
  {
    static
    constexpr
    bool supportsInsert() noexcept
    {
      return false;
    }

    /// \todo use size_type
    static
    size_t size(const MyList & list)
    {
      return list.getSizeCustom();
    }

    // template<typename UnaryPred>
    // size_t findIndexOf(UnaryPred pred) const
    // {
    //   /// \todo Here some helper to convert iterator difference to size_t ?
    // }

    // static
    // template<typename UnaryPred>
    // const_iterator findIf(UnaryPred pred, const MyList & list)
    // {
    //   return list.findItem(pred);
    // }

    // using SizeFunction = MyList::getSizeCustom;
  };

  struct MyTableModel
  {
    int rowCount() const
    {
      return mList.rowCount();
    }

    int findRowOfId(int id) const noexcept
    {
      const auto it = mList.container().findItemWithId(id);
      return mList.rowFromPosition(it);

      // const auto pred = [id](const MyItem & item) -> bool {
      //   return MyList::isRequestedItem(item, id);
      // };
      // return mList.findRowOf(pred);
    }

    bool insertRows(int row, int count)
    {
      // beginInsertRows() omitted
      return mList.insertRows( row, count, MyItem() );
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

    StlContainerAdapter<MyList, MyListTypeMap, MyListFunctionMap> mList;
  };


TEST_CASE("sandbox")
{
}


TEMPLATE_TEST_CASE("default_constructed", "", TestContainerAdapter, SharedTestContainerAdapter)
{
  TestType container;

  CHECK( container.size() == 0 );
}

TEST_CASE("rowFromPosition")
{
  REQUIRE(false);
}

TEST_CASE("rowFromIndex")
{
  REQUIRE(false);
}
