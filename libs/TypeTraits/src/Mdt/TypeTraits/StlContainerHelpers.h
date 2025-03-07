// SPDX-License-Identifier: BSL-1.0
/****************************************************************************************
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 ** Distributed under the Boost Software License,
 ** Version 1.0. (See accompanying file LICENSE_1_0.txt
 ** or copy at http://www.boost.org/LICENSE_1_0.txt)
 **
 *****************************************************************************************/
#ifndef MDT_TYPE_TRAITS_STL_CONTAINER_HELPERS_H
#define MDT_TYPE_TRAITS_STL_CONTAINER_HELPERS_H

#include "Mdt/TypeTraits/detected_or.h"
#include "Mdt/TypeTraits/is_detected.h"
#include "Mdt/TypeTraits/Impl/StlContainerHelpers.h"

namespace Mdt{ namespace TypeTraits{

  /*! \brief Returns Container::value_type if \a Container defines it, otherwise void
   */
  template<typename Container>
  using member_value_type_or_void = detected_or_t<void, Impl::member_value_type_op, Container>;

  /*! \brief Returns Container::size_type if \a Container defines it, otherwise void
   */
  template<typename Container>
  using member_size_type_or_void = detected_or_t<void, Impl::member_size_type_op, Container>;

  /*! \brief Returns Container::difference_type if \a Container defines it, otherwise void
   */
  template<typename Container>
  using member_difference_type_or_void = detected_or_t<void, Impl::member_difference_type_op, Container>;

  /*! \brief Returns Container::reference if \a Container defines it, otherwise void
   */
  template<typename Container>
  using member_reference_or_void = detected_or_t<void, Impl::member_reference_op, Container>;

  /*! \brief Returns Container::const_reference if \a Container defines it, otherwise void
   */
  template<typename Container>
  using member_const_reference_or_void = detected_or_t<void, Impl::member_const_reference_op, Container>;

  /*! \brief Returns Container::pointer if \a Container defines it, otherwise void
   */
  template<typename Container>
  using member_pointer_or_void = detected_or_t<void, Impl::member_pointer_op, Container>;

  /*! \brief Returns Container::const_pointer if \a Container defines it, otherwise void
   */
  template<typename Container>
  using member_const_pointer_or_void = detected_or_t<void, Impl::member_const_pointer_op, Container>;

  /*! \brief Returns Container::iterator if \a Container defines it, otherwise void
   */
  template<typename Container>
  using member_iterator_or_void = detected_or_t<void, Impl::member_iterator_op, Container>;

  /*! \brief Returns Container::iterator if \a Container defines it, otherwise void*
   */
  template<typename Container>
  using member_iterator_or_void_pointer = detected_or_t<void*, Impl::member_iterator_op, Container>;

  /*! \brief Returns Container::const_iterator if \a Container defines it, otherwise void
   */
  template<typename Container>
  using member_const_iterator_or_void = detected_or_t<void, Impl::member_const_iterator_op, Container>;

  /*! \brief Returns Container::const_iterator if \a Container defines it, otherwise void*
   */
  template<typename Container>
  using member_const_iterator_or_void_pointer = detected_or_t<void*, Impl::member_const_iterator_op, Container>;

  /*! \brief Returns Container::reverse_iterator if \a Container defines it, otherwise void
   */
  template<typename Container>
  using member_reverse_iterator_or_void = detected_or_t<void, Impl::member_reverse_iterator_op, Container>;

  /*! \brief Returns Container::const_reverse_iterator if \a Container defines it, otherwise void
   */
  template<typename Container>
  using member_const_reverse_iterator_or_void = detected_or_t<void, Impl::member_const_reverse_iterator_op, Container>;

  /*! \brief Check if given container has an insert member function
   *
   * Returns true if the container has an insert member function with this signature:
   * \code
   * iterator insert(const_iterator pos, size_type count, const T& value);
   * \endcode
   */
  template<typename Container>
  constexpr
  bool has_member_insert_pos_count_value() noexcept
  {
    return is_detected_v<Impl::has_insert_pos_count_value_op, Container>;
  }

  /*! \brief Check if given container has an erase member function
   *
   * Returns true if the container has an insert member function with this signature:
   * \code
   * iterator erase(const_iterator first, const_iterator last);
   * \endcode
   */
  template<typename Container>
  constexpr
  bool has_member_erase_first_last() noexcept
  {
    return is_detected_v<Impl::has_erase_first_last_op, Container>;
  }

  /*! \brief Check if type T is void or a void pointer
   */
  template<typename T>
  constexpr
  bool is_void_or_void_pointer() noexcept
  {
    return std::is_void_v< std::remove_pointer_t<T> >;
  }

}} // namespace Mdt{ namespace TypeTraits{

#endif // #ifndef MDT_TYPE_TRAITS_STL_CONTAINER_HELPERS_H
