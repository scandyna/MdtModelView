// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_STL_HELPERS_H
#define MDT_ITEM_MODEL_STL_HELPERS_H

#include "Mdt/ItemModel/StlContiguousContainerFunctionMap.h"
#include "Mdt/ItemModel/NumericLimits.h"
#include "Mdt/TypeTraits/StlContainerHelpers.h"
#include "Mdt/Numeric/Limits.h"
#include "Mdt/Numeric/BasicConversion.h"
#include <iterator>
#include <cassert>

namespace Mdt{ namespace ItemModel{

  /*! \brief Get the maximum allowed count of elements in given container
   *
   * If the function map provides a function to get the container's maximum allowed size,
   * it will be used.
   *
   * This function must be of this form:
   * \code
   * static
   * size_type maxSize(const Container & container);
   * \endcode
   *
   * Returns the minimum value between:
   * - the largest possible value for int
   * - the largest possible value for FunctionMap::size_type
   * - FunctionMap::maxSize() if defined
   *
   * \sa StlContiguousContainerAdapter
   */
  template< typename Container, typename FunctionMap = StlContiguousContainerFunctionMap<Container> >
  int stlContainerMaxElementCount(const Container & container)
  {
    if constexpr( Mdt::TypeTraits::has_member_maxSize_container<FunctionMap, Container>() ){
      return minBetweenSizeTypeValueAndIntMax( FunctionMap::maxSize(container) );
    }
    return minBetweenSizeTypeMaxAndIntMax<typename FunctionMap::size_type>();
  }

  /*! \brief Get the count of elements in given container
   *
   * \pre The size of the container must be convertible to int.
   * The result must also be >= 0 and <= stlContainerMaxElementCount()
   */
  template< typename Container, typename FunctionMap = StlContiguousContainerFunctionMap<Container> >
  int stlContainerElementCount(const Container & container)
  {
    assert( Mdt::Numeric::int_canHoldValueOf_T( FunctionMap::size(container) ) );

    const int count = Mdt::Numeric::int_from_T( FunctionMap::size(container) );
    assert( count >= 0 );
    assert( count <= stlContainerMaxElementCount(container) );

    return count;
  }

  /*! \brief Check if given \a count elements can be added to the container
   *
   * If stlContainerElementCount() + \a count can't be represented by int,
   * returns false.
   *
   * If stlContainerElementCount() + \a count exceeds stlContainerMaxElementCount(),
   * returns false.
   *
   * This also implies that stlContainerElementCount() + \a count can be represented
   * by FunctionMap::size_type .
   *
   * \todo AbstractTableModel will have the same issue to deal with
   * Provide a common helper
   *
   * \pre \a count must be >= 1
   */
  template< typename Container, typename FunctionMap = StlContiguousContainerFunctionMap<Container> >
  bool canAddCountElementsToStlContainer(const Container & container, int count)
  {
    assert( count >= 1 );

    if( !Mdt::Numeric::canAdd(stlContainerElementCount(container), count) ){
      return false;
    }

    return (stlContainerElementCount(container) + count) <= stlContainerMaxElementCount(container);
  }

  /*! \brief Inserts \a count elements into the container before the given \a index
   *
   * This is a helper to implement Qt item models.
   *
   * \pre \a index must be >= 0
   * \pre \a index must be <= container's size
   * \pre \a count must be >= 1
   * \pre it must be possible to add \a count elements to the container
   * \sa StlContiguousContainerAdapter
   *
   * \todo Fix noexcept
   */
  template< typename Container, typename FunctionMap = StlContiguousContainerFunctionMap<Container> >
  void insertToStlContainer(Container & container, int index, int count, const typename FunctionMap::const_reference value) noexcept
  {
    assert( index >= 0 );
    assert( static_cast<typename FunctionMap::size_type>(index) <= FunctionMap::size(container) );
    assert( count >= 1 );
    assert( (index + count) > 0 );

    const auto dIndex = static_cast<typename FunctionMap::difference_type>(index);
    const auto sCount = static_cast<typename FunctionMap::size_type>(count);

    auto it = std::next(FunctionMap::cbegin(container), dIndex);

    FunctionMap::insert(container, it, sCount, value);
  }

  /*! \brief Remove \a count elements starting from \a index from given container
   *
   * This is a helper to implement Qt item models.
   *
   * It deals with common boilerplate,
   * like getting iterators from given index and count,
   * checking preconditions and casting from ints.
   *
   * \pre \a index must be >= 0
   * \pre \a count must be >= 1
   * \pre ( \a index + \a count ) must be <= container's size
   * \sa StlContiguousContainerAdapter
   *
   * \todo Fix noexcept
   */
  template< typename Container, typename FunctionMap = StlContiguousContainerFunctionMap<Container> >
  void removeFromStlContainer(Container & container, int index, int count) noexcept
  {
    assert( index >= 0 );
    assert( count >= 1 );
    assert( (index + count) > 0 );
    assert( static_cast<typename FunctionMap::size_type>(index + count) <= FunctionMap::size(container) );

    const auto dIndex = static_cast<typename FunctionMap::difference_type>(index);
    const auto dCount = static_cast<typename FunctionMap::difference_type>(count);

    const auto first = std::next(FunctionMap::cbegin(container), dIndex);
    const auto last = std::next(first, dCount);

    FunctionMap::erase(container, first, last);
  }

  /*! \brief Remove the first element from given container
   *
   * \pre \a container must not be empty
   */
  template<typename Container>
  void removeFirstFromStlContainer(Container & container) noexcept
  {
    assert( !container.empty() );

    container.erase( container.cbegin() );
  }

  /*! \brief Remove the last element from given container
   *
   * \pre \a container must not be empty
   */
  template<typename Container>
  void removeLastFromStlContainer(Container & container) noexcept
  {
    assert( !container.empty() );

    const auto pos = std::prev(container.cend(), 1);

    container.erase(pos);
  }

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_STL_HELPERS_H
