// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_TEST_LIB_INSERT_ROWS_SIGNALS_SPY_H
#define MDT_ITEM_MODEL_TEST_LIB_INSERT_ROWS_SIGNALS_SPY_H

#include "mdt_itemmodel_testlib_export.h"
#include <QObject>
#include <QAbstractItemModel>
#include <cstddef>
#include <vector>
#include <cassert>

namespace Mdt{ namespace ItemModel{ namespace TestLib{

  /*! \brief Record representing a QAbstractItemModel::rowsAboutToBeInserted() signal
   */
  class MDT_ITEMMODEL_TESTLIB_EXPORT RowsAboutToBeInsertedSignalRecord
  {
   public:

    /*! \brief Construct a signal record
     */
    explicit RowsAboutToBeInsertedSignalRecord(const QModelIndex & parent, int first, int last) noexcept;

    /*! \brief Get the parent index
     */
    const QModelIndex & parentIndex() const noexcept
    {
      return mParentIndex;
    }

    /*! \brief Get the first row
     */
    int first() const noexcept
    {
      return mFirst;
    }

    /*! \brief Get the last row
     */
    int last() const noexcept
    {
      return mLast;
    }

   private:

    QModelIndex mParentIndex;
    int mFirst = 0;
    int mLast = 0;
  };

  /*! \brief Record representing a QAbstractItemModel::rowsInserted() signal
   */
  class MDT_ITEMMODEL_TESTLIB_EXPORT RowsInsertedSignalRecord
  {
   public:

    /*! \brief Construct a signal record
     */
    explicit RowsInsertedSignalRecord(const QModelIndex & parent, int first, int last) noexcept;

    /*! \brief Get the parent index
     */
    const QModelIndex & parentIndex() const noexcept
    {
      return mParentIndex;
    }

    /*! \brief Get the first row
     */
    int first() const noexcept
    {
      return mFirst;
    }

    /*! \brief Get the last row
     */
    int last() const noexcept
    {
      return mLast;
    }

   private:

    QModelIndex mParentIndex;
    int mFirst = 0;
    int mLast = 0;
  };

  /*! \brief Check if both records contains the same arguments
   */
  MDT_ITEMMODEL_TESTLIB_EXPORT
  bool operator==(const RowsAboutToBeInsertedSignalRecord & rowsAboutToBeInsertedRecord, const RowsInsertedSignalRecord & rowsInsertedRecord) noexcept;

  /*! \brief Check if both records contains the same arguments
   */
  inline
  bool operator==(const RowsInsertedSignalRecord & rowsInsertedRecord, const RowsAboutToBeInsertedSignalRecord & rowsAboutToBeInsertedRecord) noexcept
  {
    return rowsAboutToBeInsertedRecord == rowsInsertedRecord;
  }

  inline
  bool operator!=(const RowsAboutToBeInsertedSignalRecord & rowsAboutToBeInsertedRecord, const RowsInsertedSignalRecord & rowsInsertedRecord) noexcept
  {
    return !(rowsAboutToBeInsertedRecord == rowsInsertedRecord);
  }

  inline
  bool operator!=(const RowsInsertedSignalRecord & rowsInsertedRecord, const RowsAboutToBeInsertedSignalRecord & rowsAboutToBeInsertedRecord) noexcept
  {
    return rowsAboutToBeInsertedRecord != rowsInsertedRecord;
  }

  /*! \brief Enables introspection of signals for rows insertions for a QAbstractItemModel based model
   *
   * \code
   * MyModel model;
   * InsertRowsSignalsSpy spy(model);
   *
   * appendRowToModel(model);
   *
   * REQUIRE( spy.rowsAboutToBeInsertedCount() == 1 );
   * REQUIRE( spy.rowsInsertedCount() == 1 );
   *
   * const auto rowsInserted = spy.rowsInsertedAt(0);
   * REQUIRE( spy.rowsAboutToBeInsertedAt(0) == rowsInserted );
   * REQUIRE( !rowsInserted.parentIndex().isValid() );
   * REQUIRE( rowsInserted.first() == 0 );
   * REQUIRE( rowsInserted.last() == 0 );
   * \endcode
   *
   */
  class MDT_ITEMMODEL_TESTLIB_EXPORT InsertRowsSignalsSpy : public QObject
  {
   Q_OBJECT

   public:

    /*! \brief Constructor
     */
    explicit InsertRowsSignalsSpy(QAbstractItemModel & model) noexcept;

    /*! \brief Get the count of rowsAboutToBeInserted signals this spy received
     */
    size_t rowsAboutToBeInsertedCount() const noexcept
    {
      return mRowsAboutToBeInsertedList.size();
    }

    /*! \brief Get the count of rowsInserted signals this spy received
     */
    size_t rowsInsertedCount() const noexcept
    {
      return mRowsInsertedList.size();
    }

    /*! \brief Get the rowsAboutToBeInserted signal record at given index
     *
     * \pre \a index must be in valid range ( \a index < rowsAboutToBeInsertedCount() )
     */
    const RowsAboutToBeInsertedSignalRecord & rowsAboutToBeInsertedAt(size_t index) const noexcept
    {
      assert( index < rowsAboutToBeInsertedCount() );

      return mRowsAboutToBeInsertedList[index];
    }

    /*! \brief Get the rowsInserted signal record at given index
     *
     * \pre \a index must be in valid range ( \a index < rowsInsertedCount() )
     */
    const RowsInsertedSignalRecord & rowsInsertedAt(size_t index) const noexcept
    {
      assert( index < rowsInsertedCount() );

      return mRowsInsertedList[index];
    }

   private slots:

    void addRowsAboutToBeInsertedRecord(const QModelIndex & parent, int first, int last) noexcept;
    void addRowsInsertedRecord(const QModelIndex & parent, int first, int last) noexcept;

   private:

    std::vector<RowsAboutToBeInsertedSignalRecord> mRowsAboutToBeInsertedList;
    std::vector<RowsInsertedSignalRecord> mRowsInsertedList;
  };

}}} // namespace Mdt{ namespace ItemModel{ namespace TestLib{

#endif // #ifndef MDT_ITEM_MODEL_TEST_LIB_INSERT_ROWS_SIGNALS_SPY_H
