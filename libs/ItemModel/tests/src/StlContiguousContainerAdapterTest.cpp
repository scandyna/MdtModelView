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

  template< typename Container, typename FunctionMap = StlContiguousContainerFunctionMap<Container> >
  struct Adapter
  {
    using const_reference = typename FunctionMap::const_reference;
    // using const_reference = typename Container::const_reference;

    int rowCount() const
    {
      return FunctionMap::size(mContainer);
      // return std::invoke(FunctionMap::GetSize, mContainer);
      // return std::invoke(FunctionMap::SizeFunction, mContainer);
    }

    const_reference atRow(int row) const noexcept
    {
    }

    void atRowMutable(int row) noexcept
    {
    }

    template<typename UnaryPred>
    int findRowOf(UnaryPred pred) const
    {
      /// todo check provides iterator
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
  };

  // using MyListFunctionMap = Xy_FunctionMap<MyList>;

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


    // using SizeFunction = MyList::getSizeCustom;
  };

  struct MyTableModel
  {
    int rowCount() const
    {
      return mList.rowCount();
    }

    int findRowOfName(const QString & name) const noexcept
    {
      const auto pred = [&name](const MyItem & item) -> bool {
        return item.name == name;
      };
      return mList.findRowOf(pred);
    }

    // bool insert()
    // {
    //   return mList.insert();
    // }

    Adapter<MyList, MyListFunctionMap> mList;
  };


TEST_CASE("sandbox")
{
}


TEMPLATE_TEST_CASE("default_constructed", "", TestContainerAdapter, SharedTestContainerAdapter)
{
  TestType container;

  CHECK( container.size() == 0 );
}
