// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_REVERSE_ROW_LIST_VIEW_H
#define MDT_ITEM_MODEL_REVERSE_ROW_LIST_VIEW_H

#include "Mdt/ItemModel/RowRangeList.h"
#include "Mdt/ItemModel/RowSelection.h"
#include "Mdt/ItemModel/ReverseRowListViewConstIterator.h"
#include "mdt_itemmodel_export.h"

namespace Mdt{ namespace ItemModel{

  /*! \brief View that acts as a list of rows
   *
   * To remove a list of items from a collection,
   * a good choice is to use STL erase().
   * But, if we have a list of rows comming from a selection,
   * we probably don't have a full range.
   * As example, the user may select rows 1,3,4.
   *
   * In such case, we have to remove the elements starting from the end:
   * \code
   * RowSelection rowSelection = ...
   * ReverseRowListView rowList(rowSelection);
   *
   * for(int row : rowList){
   *   removeElement(row);
   * }
   * \endcode
   *
   * \warning A ReverseRowListView is only valid as long as
   * the range list or selection it refers to is valid.
   * RowListView should be used as short life objects,
   * and should not be stored as class member.
   *
   * \sa RowSelection
   * \sa RowListView
   */
  class MDT_ITEMMODEL_EXPORT ReverseRowListView
  {
   public:

    /*! \brief STL style const iterator
     *
     * Dereferencing this iterator returns an int
     */
    using const_iterator = ReverseRowListViewConstIterator;

    /*! \brief STL value_type
     */
    using value_type = ReverseRowListViewConstIterator::value_type;

    /*! \brief Construct a view that acts on given list of ranges
     */
    explicit
    ReverseRowListView(const RowRangeList & rangeList) noexcept
     : mBegin( rangeList.crbegin() ),
       mEnd( rangeList.crend() )
    {
    }

    /*! \brief Construct a view that acts on given selection
     */
    explicit
    ReverseRowListView(const RowSelection & selection) noexcept
     : mBegin( selection.crbegin() ),
       mEnd( selection.crend() )
    {
    }

    /*! \brief Get a const iterator to the first row in this list
     */
    const_iterator cbegin() const noexcept
    {
      return mBegin;
    }

    /*! \brief Get a const iterator past the end in this list
     */
    const_iterator cend() const noexcept
    {
      return mEnd;
    }

    /*! \brief Get a const iterator to the first row in this list
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

   private:

//     RowListView(const_iterator begin, const_iterator end) noexcept
//      : mBegin(begin),
//        mEnd(end)
//     {
//     }

    const_iterator mBegin;
    const_iterator mEnd;
  };

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_REVERSE_ROW_LIST_VIEW_H
