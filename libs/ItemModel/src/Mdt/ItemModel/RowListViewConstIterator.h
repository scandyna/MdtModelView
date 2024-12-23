// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_ROW_LIST_VIEW_CONST_ITERATOR_H
#define MDT_ITEM_MODEL_ROW_LIST_VIEW_CONST_ITERATOR_H

#include "Mdt/ItemModel/RowRangeListDef.h"
#include "Mdt/ItemModel/RowRange.h"
#include "mdt_itemmodel_export.h"
#include <iterator>
#include <cstddef>

namespace Mdt{ namespace ItemModel{

  /*! \brief Iterate over a list of row ranges as it was a list of rows
   *
   * RowRangeList and RowSelection holds a list of RowRange (row intervals).
   * While they can save memory for large amount of rows,
   * they not provide a way to iterate over all rows that they represent.
   *
   * Usage example:
   * \code
   * RowSelection rowSelection = ...
   * RowListViewConstIterator first( rowSelection.cbegin() );
   * RowListViewConstIterator last( rowSelection.cend() );
   *
   * while(first != last){
   *   // Dereferencing first returns a row (an int)
   *   doSomething(*first);
   *   ++first;
   * }
   * \endcode
   *
   * Incrementing the iterator will go to the next row in the range it refers to.
   * Once the last row of the current range has been reached,
   * incrementing will refer to the next range.
   *
   * \sa RowListView
   */
  class MDT_ITEMMODEL_EXPORT RowListViewConstIterator
  {
   public:

    /*! \brief STL iterator value_type
     */
    using value_type = int;

    /*! \brief STL iterator difference_type
     */
    using difference_type = std::ptrdiff_t;

    /*! \brief STL iterator reference
     */
    using reference = int&;

    /*! \brief STL iterator pointer
     */
    using pointer = int*;

    /*! \brief STL iterator iterator_category
     */
    using iterator_category = std::input_iterator_tag;

    /*! \brief Construct a row list iterator that references given range list iterator
     */
    explicit
    RowListViewConstIterator(RowRangeListConstIterator it) noexcept
     : mRowRangeListIterator(it)
    {
    }

    /*! \brief Copy construct an iterator from \a other
     */
    constexpr
    RowListViewConstIterator(const RowListViewConstIterator & other) noexcept = default;

    /*! \brief Copy assign \a other to this iterator
     */
    RowListViewConstIterator & operator=(const RowListViewConstIterator & other) noexcept = default;

    /*! \brief Move construct an iterator from \a other
     */
    constexpr
    RowListViewConstIterator(RowListViewConstIterator && other) noexcept = default;

    /*! \brief Move assign \a other to this iterator
     */
    RowListViewConstIterator & operator=(RowListViewConstIterator && other) noexcept = default;

    /*! \brief Get the current row
     */
    int operator*() const noexcept
    {
      if(mCurrentRow < 0){
        return mRowRangeListIterator->firstRow();
      }
      return mCurrentRow;
    }

    /*! \brief Increment this iterator (pre-increment)
     */
    RowListViewConstIterator & operator++() noexcept
    {
      if(mCurrentRow < 0){
        mCurrentRow = mRowRangeListIterator->firstRow();
      }
      if( mCurrentRow == mRowRangeListIterator->lastRow() ){
        mCurrentRow = -1;
        ++mRowRangeListIterator;
      }else{
        ++mCurrentRow;
      }

      return *this;
    }

    /*! \brief Increment this iterator (post-increment)
     *
     * \sa operator++()
     */
    RowListViewConstIterator operator++(int) noexcept
    {
      RowListViewConstIterator old = *this;
      operator++();
      return old;
    }

    /*! \brief Check if iterators \a a and \a b are equal
     */
    friend
    bool operator==(const RowListViewConstIterator & a, const RowListViewConstIterator & b) noexcept
    {
      return (a.mRowRangeListIterator == b.mRowRangeListIterator) && (a.mCurrentRow == b.mCurrentRow);
    }

    /*! \brief Check if iterators \a a and \a b are not equal
     */
    friend
    bool operator!=(const RowListViewConstIterator & a, const RowListViewConstIterator & b) noexcept
    {
      return !(a == b);
    }

   private:

    bool isDereferencable() const noexcept
    {
      return mRowRangeListIterator != RowRangeListConstIterator{};
    }

    int mCurrentRow = -1;
    RowRangeListConstIterator mRowRangeListIterator;
  };

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_ROW_LIST_VIEW_CONST_ITERATOR_H
