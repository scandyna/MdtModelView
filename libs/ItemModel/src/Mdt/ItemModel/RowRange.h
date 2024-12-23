// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_ROW_RANGE_H
#define MDT_ITEM_MODEL_ROW_RANGE_H

#include "mdt_itemmodel_export.h"
#include <cassert>

namespace Mdt{ namespace ItemModel{

  /*! \brief Represents a range of contiguous rows
   *
   * As an example, the range [0,2]
   * represents the rows {0,1,2}.
   *
   * While a range is represented,
   * only the first and last row are stored.
   * This can be important to represent a range of selected rows
   * in an item model accessing a large collection.
   *
   * \note The last row is part of a range of rows
   * (it is NOT past the end).
   *
   * \sa RowSelection
   * \sa RowRangeList
   *
   * \section Rationale
   *
   * In a formal way, this class represents an interval of rows.
   * It maybe should have been named RowIterval.
   *
   * After searching a bit, I saw that name range could also be used to represent an interval.
   * I have finally chosen RowRange to match the naming in Qt, QItemSelectionRange .
   */
  class MDT_ITEMMODEL_EXPORT RowRange
  {
   public:

    /*! \brief An empty range does not exist
     */
    RowRange() = delete;

    /*! \brief Copy construct a row range from \a other
     */
    constexpr
    RowRange(const RowRange & other) noexcept = default;

    /*! \brief Copy assign \a other to this row range
     */
    constexpr
    RowRange & operator=(const RowRange & other) noexcept = default;

    /*! \brief Move construct a row range from \a other
     */
    constexpr
    RowRange(RowRange && other) noexcept = default;

    /*! \brief Move assign \a other to this row range
     */
    constexpr
    RowRange & operator=(RowRange && other) noexcept = default;

    /*! \brief Get first row
     */
    constexpr
    int firstRow() const noexcept
    {
      return mFirstRow;
    }

    /*! \brief Get the last row
     */
    constexpr
    int lastRow() const noexcept
    {
      return mLastRow;
    }

    /*! \brief Get row count
     */
    constexpr
    int rowCount() const noexcept
    {
      return mLastRow - mFirstRow + 1;
    }

    /*! \brief Check if given first and last row represents a valid range of rows
     */
    static
    constexpr
    bool firstAndLastRowIsValidRange(int firstRow, int lastRow) noexcept
    {
      if(firstRow < 0){
        return false;
      }
      if(lastRow < 0){
        return false;
      }
      if(firstRow > lastRow){
        return false;
      }

      return true;
    }

    /*! \brief Construct a row range from given first and last row
     *
     * \pre \a firstRow and \a lastRow must represent a valid range of rows
     * \sa firstAndLastRowIsValidRange()
     */
    static
    constexpr
    RowRange fromFirstAndLastRow(int firstRow, int lastRow) noexcept
    {
      assert( firstAndLastRowIsValidRange(firstRow, lastRow) );

      return RowRange(firstRow, lastRow);
    }

   private:

    constexpr
    RowRange(int firstRow, int lastRow) noexcept
     : mFirstRow(firstRow),
       mLastRow(lastRow)
    {
    }

    int mFirstRow;
    int mLastRow;
  };

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_ROW_RANGE_H
