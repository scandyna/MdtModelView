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
#include <iterator>
#include <cassert>

namespace Mdt{ namespace ItemModel{

  /*! \brief Inserts \a count elements into the container before the given \a index
   *
   * This is a helper to implement Qt item models.
   *
   * \pre \a index must be >= 0
   * \pre \a index must be <= container's size
   * \pre \a count must be >= 1
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
