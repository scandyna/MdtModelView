// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_ROW_SELECTION_H
#define MDT_ITEM_MODEL_ROW_SELECTION_H

#include "Mdt/ItemModel/RowRange.h"
#include "Mdt/ItemModel/RowRangeList.h"
#include "mdt_itemmodel_export.h"
#include <QItemSelection>
#include <cassert>

namespace Mdt{ namespace ItemModel{

  /*! \brief Selection of rows in an item model
   *
   * RowSelection holds a sorted list of row ranges that are all disjoint.
   *
   * Depending on how QItemSelection is used, it can contain the same row many times.
   * Also, it can contain 2 ranges (QItemSelectionRange) with contiguous rows.
   *
   * As an example, if we select item 0,0 then item 0,1 in this table:
   * |0|1|2|
   * |-|-|-|
   * |S|S| |
   * we end up with 2 ranges: [0,0 to 0,0] and [0,1 to 0,1].
   *
   * This should end up to a single range of rows: [0 to 0]
   *
   * Selecting items like this:
   * |0|1|2|
   * |-|-|-|
   * |S| | |
   * | |S| |
   * | | |S|
   * represents a single row range: [0 to 2]
   *
   * This example:
   * |0|1|2|
   * |-|-|-|
   * |x| | |
   * | |x| |
   * | | | |
   * |x| | |
   * |x| |x|
   * |x| | |
   * represents 2 ranges of rows: [0 to 1] and [3 to 5]
   *
   *
   * Example to remove rows from an item model:
   * \code
   * QTableView view;
   * // Setup view
   *
   * auto *model = view.model();
   * const QItemSelection itemSelection = view.selectionModel()->selection();
   * const auto rowSelection = RowSelection::fromItemSelection(itemSelection);
   *
   * auto rFirst = rowSelection.crbegin();
   * const auto rLast = rowSelection.crend();
   *
   * while(rFirst != rLast){
   *   const RowRange & range = *rFirst;
   *
   *   if( !model.removeRows( range.firstRow(), range.rowCount() ) ){
   *     // Error handling
   *   }
   *
   *   ++rFirst;
   * }
   * \endcode
   *
   * \sa Mdt::ItemModel::removeSelectedRows()
   * \sa Mdt::ItemView::removeSelectedRows()
   * \sa RowListView
   * \sa ReverseRowListView
   * \sa RowRange
   */
  class MDT_ITEMMODEL_EXPORT RowSelection
  {
   public:

    /*! \brief STL style const iterator
     *
     * Dereferencing this iterator returns a RowRange
     */
    using const_iterator = RowRangeList::const_iterator;

    /*! \brief STL style const reverse iterator
     *
     * Dereferencing this iterator returns a RowRange
     */
    using const_reverse_iterator = RowRangeList::const_reverse_iterator;

    /*! \brief Construct an empty row selection
     */
    RowSelection() noexcept = default;

    /*! \brief Copy construct a row selection from \a other
     */
    RowSelection(const RowSelection & other) noexcept = default;

    /*! \brief Copy assign \a other to this row selection
     */
    RowSelection & operator=(const RowSelection & other) noexcept = default;

    /*! \brief Move construct a row selection from \a other
     */
    RowSelection(RowSelection && other) noexcept = default;

    /*! \brief Move assign \a other to this row selection
     */
    RowSelection & operator=(RowSelection && other) noexcept = default;

    /*! \brief Check if this selection is empty
     */
    bool isEmpty() const noexcept
    {
      return mRowRangeList.isEmpty();
    }

    /*! \brief Get the count of ranges of rows this selection holds
     */
    size_t rangeCount() const noexcept
    {
      return mRowRangeList.rangeCount();
    }

    /*! \brief Get the range at \a index
     *
     * \pre \a index must be in valid range ( \a index < rangeCount() ).
     */
    const RowRange & rangeAt(size_t index) const noexcept
    {
      assert( index < rangeCount() );

      return mRowRangeList.rangeAt(index);
    }

    /*! \brief Get a const iterator to the first range in this row selection
     */
    const_iterator cbegin() const noexcept
    {
      return mRowRangeList.cbegin();
    }

    /*! \brief Get a const iterator past the end in this row selection
     */
    const_iterator cend() const noexcept
    {
      return mRowRangeList.cend();
    }

    /*! \brief Get a const iterator to the first range in this row selection
     */
    const_iterator begin() const noexcept
    {
      return cbegin();
    }

    /*! \brief Get a const iterator past the end in this row selection
     */
    const_iterator end() const noexcept
    {
      return cend();
    }

    /*! \brief Get a reverse iterator to the first element of the reversed row selection
     *
     * It corresponds to the last element of the non-reversed list.
     * If the list is empty, the returned iterator is equal to crend().
     */
    const_reverse_iterator crbegin() const noexcept
    {
      return mRowRangeList.crbegin();
    }

    /*! \brief Get a reverse iterator to the element following the last element of the reversed row selection
     *
     * It corresponds to the element preceding the first element of the non-reversed list.
     * This element acts as a placeholder, attempting to access it results in undefined behavior.
     */
    const_reverse_iterator crend() const noexcept
    {
      return mRowRangeList.crend();
    }

    /*! \brief Get a row selection from given item selection
     *
     * Returns a row selection holding a list of disjoint rows ranges.
     *
     * Depending on how QItemSelection is used, it can contain the same row many times.
     * Also, it can contain 2 ranges (QItemSelectionRange) with contiguous rows.
     *
     * As an example, if we select item 0,0 then item 0,1 in this table:
     * |0|1|2|
     * |-|-|-|
     * |S|S| |
     * we end up with 2 ranges of indexes: [(0,0),(0,0)] and [(0,1),(0,1)].
     *
     * \note Above are ranges of indexes: [(topLeft),(bottomRight)].
     *
     * This should end up to a single range of rows: [0]
     *
     * Selecting items like this:
     * |0|1|2|
     * |-|-|-|
     * |S| | |
     * | |S| |
     * | | |S|
     * represents a single row range: [0,2]
     *
     * This example:
     * |0|1|2|
     * |-|-|-|
     * |S| | |
     * | |S| |
     * | | | |
     * |S| | |
     * |S| |S|
     * |S| | |
     * represents 2 ranges of rows: [0,1] and [3,5]
     */
    static
    RowSelection fromItemSelection(const QItemSelection & itemSelection) noexcept;

   private:

    RowRangeList mRowRangeList;
  };

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_ROW_SELECTION_H
