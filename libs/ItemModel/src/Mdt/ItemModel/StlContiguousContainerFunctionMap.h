// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_STL_CONTIGUOUS_CONTAINER_FUNCTION_MAP_H
#define MDT_ITEM_MODEL_STL_CONTIGUOUS_CONTAINER_FUNCTION_MAP_H

#include <cassert>

namespace Mdt{ namespace ItemModel{

  /*! \brief STL ContiguousContainer function map for STL adapters
   *
   * \todo add maxSize()
   *
   * \sa StlContiguousContainerAdapter
   * \sa https://en.cppreference.com/w/cpp/named_req/ContiguousContainer
   */
  template<typename Container>
  struct StlContiguousContainerFunctionMap
  {
    /*! \brief STL size_type
     */
    using size_type = typename Container::size_type;

    /*! \brief STL difference_type
     */
    using difference_type = typename Container::difference_type;

    /*! \brief STL reference
     */
    using reference = typename Container::reference;

    /*! \brief STL const_reference
     */
    using const_reference = typename Container::const_reference;

    /*! \brief STL const_iterator
     */
    using const_iterator = typename Container::const_iterator;

    /*! \brief Get the size of the container
     */
    static
    size_type size(const Container & container) noexcept
    {
      return container.size();
    }

    /*! \brief Get the maximum size of the container
     */
    static
    size_type maxSize(const Container & container) noexcept
    {
      return container.max_size();
    }

    /*! \brief Get the element at given index
     *
     * \pre \a index must be in range of the container ( 0 >= \a index < container.size() )
     */
    static
    const_reference atIndex(const Container & container, size_type index) noexcept
    {
      assert( index >= 0 );
      assert( index < container.size() );

      return container[index];
    }

    /*! \brief Access the element at given index for mutation
     *
     * \pre \a index must be in range of the container ( 0 >= \a index < container.size() )
     */
    static
    reference atIndexMutable(Container & container, size_type index) noexcept
    {
      assert( index >= 0 );
      assert( index < container.size() );

      return container[index];
    }

    /*! \brief Returns a const_iterator to the first element of the container
     */
    static
    const_iterator cbegin(const Container & container) noexcept
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

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_STL_CONTIGUOUS_CONTAINER_FUNCTION_MAP_H
