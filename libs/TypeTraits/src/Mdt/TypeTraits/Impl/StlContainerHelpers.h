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
#ifndef MDT_TYPE_TRAITS_IMPL_STL_CONTAINER_HELPERS_H
#define MDT_TYPE_TRAITS_IMPL_STL_CONTAINER_HELPERS_H

#include "Mdt/TypeTraits/detected_or.h"
#include "Mdt/TypeTraits/is_detected.h"
#include <utility>

namespace Mdt{ namespace TypeTraits{ namespace Impl{

  /*! \internal
   */
  template<typename Container>
  using member_value_type_op = typename Container::value_type;

  /*! \internal
   */
  template<typename Container>
  using member_size_type_op = typename Container::size_type;

  /*! \internal
   */
  template<typename Container>
  using member_difference_type_op = typename Container::difference_type;

  /*! \internal
   */
  template<typename Container>
  using member_reference_op = typename Container::reference;

  /*! \internal
   */
  template<typename Container>
  using member_const_reference_op = typename Container::const_reference;

  /*! \internal
   */
  template<typename Container>
  using member_pointer_op = typename Container::pointer;

  /*! \internal
   */
  template<typename Container>
  using member_const_pointer_op = typename Container::const_pointer;

  /*! \internal
   */
  template<typename Container>
  using member_iterator_op = typename Container::iterator;

  /*! \internal
   */
  template<typename Container>
  using member_const_iterator_op = typename Container::const_iterator;

  /*! \internal
   */
  template<typename Container>
  using member_reverse_iterator_op = typename Container::reverse_iterator;

  /*! \internal
   */
  template<typename Container>
  using member_const_reverse_iterator_op = typename Container::const_reverse_iterator;

  /*! \internal
   */
  template<typename Container>
  using has_insert_pos_count_value_op = decltype(
    std::declval<Container>().insert( std::declval<typename Container::const_iterator>(),
                                      std::declval<typename Container::size_type>(),
                                      std::declval<typename Container::const_reference>() )
  );

  /*! \internal
   */
  template<typename Container>
  using has_erase_first_last_op = decltype(
    std::declval<Container>().erase( std::declval<typename Container::const_iterator>(),
                                     std::declval<typename Container::const_iterator>() )
  );

}}} // namespace Mdt{ namespace TypeTraits{ namespace Impl{

#endif // #ifndef MDT_TYPE_TRAITS_IMPL_STL_CONTAINER_HELPERS_H
