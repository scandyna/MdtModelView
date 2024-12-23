// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_ROW_LIST_VIEW_H
#define MDT_ITEM_MODEL_ROW_LIST_VIEW_H

#include "Mdt/ItemModel/RowRangeList.h"
#include "Mdt/ItemModel/RowSelection.h"
#include "Mdt/ItemModel/RowListViewConstIterator.h"
#include "mdt_itemmodel_export.h"

namespace Mdt{ namespace ItemModel{

  /*! \brief View that acts as a list of rows
   *
   * \code
   * RowSelection rowSelection = ...
   * RowListView rowList(rowSelection);
   *
   * for(int row : rowList){
   *   doSomething(row);
   * }
   * \endcode
   *
   * \warning A RowListView is only valid as long as
   * the range list or selection it refers to is valid.
   * RowListView should be used as short life objects,
   * and should not be stored as class member.
   *
   * \note RowListView provides no facility like rowCount() or rowAt() ,
   * because this would hide the fact that iterating over this view is not constant time.
   *
   * \sa RowSelection
   * \sa ReverseRowListView
   */
  class MDT_ITEMMODEL_EXPORT RowListView
  {
   public:

    /*! \brief STL style const iterator
     *
     * Dereferencing this iterator returns an int
     */
    using const_iterator = RowListViewConstIterator;

    /*! \brief STL value_type
     */
    using value_type = RowListViewConstIterator::value_type;

    /*! \brief Construct a view that acts on given list of ranges
     */
    explicit
    RowListView(const RowRangeList & rangeList) noexcept
     : mBegin( rangeList.cbegin() ),
       mEnd( rangeList.cend() )
    {
    }

    /*! \brief Construct a view that acts on given selection
     */
    explicit
    RowListView(const RowSelection & selection) noexcept
     : mBegin( selection.cbegin() ),
       mEnd( selection.cend() )
    {
    }

    /*! \brief Check if the referenced ranges represents an empty list of rows
     */
    bool isEmpty() const noexcept
    {
      return cbegin() == cend();
    }

    /*! \brief Check if the referenced ranges represents a single row
     */
    bool isSingleRow() const noexcept
    {
      if( isEmpty() ){
        return false;
      }
      auto it = cbegin();
      ++it;

      return it == cend();
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

    const_iterator mBegin;
    const_iterator mEnd;
  };

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_ROW_LIST_VIEW_H
