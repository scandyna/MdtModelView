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

    /*! \brief STL ContiguousContainer function map for STL adapters
     *
     * \todo Maybe inheritance like iterators ?
     *
     * \sa https://en.cppreference.com/w/cpp/named_req/ContiguousContainer
     */
    template<typename Container>
    struct StlContiguousContainerFunctionMap
    {
      /*! \brief
       *
       * \todo propose a indexOf() based on find_if()
       */
      
      /// \todo below seems wrong
      
      /*! \brief STL const_iterator
       */
      using const_iterator = typename Container::const_iterator;

      /*! \brief Check if const_iterator is provided
       *
       * Returns true,
       * because const_iterator, cbegin() and cend() are provided
       * by an STL conform ContiguousContainer .
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

    };

  /*! \brief
   *
   * \todo Maybe StlContiguousContainerTypeMap
   */
  template<typename Container>
  struct StlContainerTypeMap
  {
    /*! \brief STL size_type
     */
    using size_type = typename Container::size_type;

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
    typename TypeMap,
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

    /*! \brief Get the row that satisfies given predicate
     *
     * Returns the row if an element was found,
     * otherwise a value < 0
     *
     * If the container does not provide const_iterator,
     * a value < 0 is always returned.
     *
     * \todo Maybe better in a function map ? (!)
     *
     * \param pred unary predicate which returns ​true for the required element
     * \sa https://en.cppreference.com/w/cpp/algorithm/find
     */
    template<typename UnaryPred>
    int findRowOf(UnaryPred pred) const
    {
      if constexpr( !std::is_void_v<const_iterator> ){
        const auto it = mContainer.cbegin();
        
      }
      return -1;
    }

    /// \todo See std::function() interface
    template<typename Function>
    int findRowOf() const
    {
      
    }

    /// get data
    bool insert()
    {
      if constexpr( FunctionMap::supportsInsert() ){
        return FunctionMap::insert(mContainer);
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

  struct MyList
  {
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

    static
    size_t size(const MyList & list)
    {
      return list.getSizeCustom();
    }

    template<typename UnaryPred>
    size_t findIndexOf(UnaryPred pred) const
    {
      /// \todo Here some helper to convert iterator difference to size_t ?
    }

    static
    template<typename UnaryPred>
    const_iterator findIf(UnaryPred pred, const MyList & list)
    {
      return list.findItem(pred);
    }

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
      const auto pred = [id](const MyItem & item) -> bool {
        return MyList::isRequestedItem(item, id);
      };
      return mList.findRowOf(pred);
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
