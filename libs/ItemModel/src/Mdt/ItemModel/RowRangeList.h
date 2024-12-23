// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_ROW_RANGE_LIST_H
#define MDT_ITEM_MODEL_ROW_RANGE_LIST_H

#include "Mdt/ItemModel/RowRangeListDef.h"
#include "Mdt/ItemModel/RowRange.h"
#include "mdt_itemmodel_export.h"
#include <cassert>

namespace Mdt{ namespace ItemModel{

  /*! \brief Represents a sorted list of RowRange
   *
   * As an example, the list {[0,2],[5,10]}
   * represents the rows {0,1,2,5,6,7,8,9,10}.
   *
   * \sa RowSelection
   * \sa RowRange
   */
  class MDT_ITEMMODEL_EXPORT RowRangeList
  {
   public:

    /*! \brief STL style const iterator
     *
     * Dereferencing this iterator returns a RowRange
     */
    using const_iterator = RowRangeListContainer::const_iterator;

    /*! \brief STL style const reverse iterator
     *
     * Dereferencing this iterator returns a RowRange
     */
    using const_reverse_iterator = RowRangeListContainer::const_reverse_iterator;

    /*! \brief Check if this list is empty
     */
    bool isEmpty() const noexcept
    {
      return mList.empty();
    }

    /*! \brief Get the count of ranges of rows this list holds
     */
    size_t rangeCount() const noexcept
    {
      return mList.size();
    }

    /*! \brief Get the range at \a index
     *
     * \pre \a index must be in valid range ( \a index < rangeCount() ).
     */
    const RowRange & rangeAt(size_t index) const noexcept
    {
      assert( index < rangeCount() );

      return mList[index];
    }

    /*! \brief Add a range to this list
     *
     * Given range will be added in a way
     * so that this list is sorted.
     *
     * As an example, starting with this list:
     * {[0,1],[6,8]}
     * adding the range [3,4] will result in:
     * {[0,1],[3,4],[6,8]}.
     *
     * If given range is adjacent to an existing one,
     * it will be merged to form a contiguous range.
     *
     * As an example, starting with this list:
     * {[0,1],[6,8]}
     * adding the range [2,4] will result in:
     * {[0,4],[6,8]}.
     *
     * The same is done if given range overlaps with an existing one.
     *
     * As an example, starting this list:
     * {[0,1],[6,8]}
     * adding the range [0,4] will result in:
     * {[0,4],[6,8]}.
     */
    void addRange(const RowRange & range) noexcept;

    /*! \brief Get a const iterator to the first range in this list
     */
    const_iterator cbegin() const noexcept
    {
      return mList.cbegin();
    }

    /*! \brief Get a const iterator past the end in this list
     */
    const_iterator cend() const noexcept
    {
      return mList.cend();
    }

    /*! \brief Get a const iterator to the first range in this list
     */
    const_iterator begin() const noexcept
    {
      return cbegin();
    }

    /*! \brief Get a const iterator past the end in this list
     */
    const_iterator end() const noexcept
    {
      return cend();
    }

    /*! \brief Get a reverse iterator to the first element of the reversed list
     *
     * It corresponds to the last element of the non-reversed list.
     * If the list is empty, the returned iterator is equal to crend().
     */
    const_reverse_iterator crbegin() const noexcept
    {
      return mList.crbegin();
    }

    /*! \brief Get a reverse iterator to the element following the last element of the reversed list
     *
     * It corresponds to the element preceding the first element of the non-reversed list.
     * This element acts as a placeholder, attempting to access it results in undefined behavior.
     */
    const_reverse_iterator crend() const noexcept
    {
      return mList.crend();
    }

   private:

    RowRangeListContainer mList;
  };

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_ROW_RANGE_LIST_H
