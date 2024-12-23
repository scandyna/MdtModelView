// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_TEST_LIB_REMOVE_ROWS_SIGNALS_SPY_H
#define MDT_ITEM_MODEL_TEST_LIB_REMOVE_ROWS_SIGNALS_SPY_H

#include "mdt_itemmodel_testlib_export.h"
#include <QObject>
#include <QAbstractItemModel>
#include <cstddef>
#include <vector>
#include <cassert>

namespace Mdt{ namespace ItemModel{ namespace TestLib{

  /*! \brief Record representing a QAbstractItemModel::rowsAboutToBeRemoved() signal
   */
  class MDT_ITEMMODEL_TESTLIB_EXPORT RowsAboutToBeRemovedSignalRecord
  {
   public:

    /*! \brief Construct a signal record
     */
    explicit RowsAboutToBeRemovedSignalRecord(const QModelIndex & parent, int first, int last) noexcept;

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

  /*! \brief Record representing a QAbstractItemModel::rowsRemoved() signal
   */
  class MDT_ITEMMODEL_TESTLIB_EXPORT RowsRemovedSignalRecord
  {
   public:

    /*! \brief Construct a signal record
     */
    explicit RowsRemovedSignalRecord(const QModelIndex & parent, int first, int last) noexcept;

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
  bool operator==(const RowsAboutToBeRemovedSignalRecord & rowsAboutToBeRemovedRecord, const RowsRemovedSignalRecord & rowsRemovedRecord) noexcept;

  /*! \brief Check if both records contains the same arguments
   */
  inline
  bool operator==(const RowsRemovedSignalRecord & rowsRemovedRecord, const RowsAboutToBeRemovedSignalRecord & rowsAboutToBeRemovedRecord) noexcept
  {
    return rowsAboutToBeRemovedRecord == rowsRemovedRecord;
  }

  inline
  bool operator!=(const RowsAboutToBeRemovedSignalRecord & rowsAboutToBeRemovedRecord, const RowsRemovedSignalRecord & rowsRemovedRecord) noexcept
  {
    return !(rowsAboutToBeRemovedRecord == rowsRemovedRecord);
  }

  inline
  bool operator!=(const RowsRemovedSignalRecord & rowsRemovedRecord, const RowsAboutToBeRemovedSignalRecord & rowsAboutToBeRemovedRecord) noexcept
  {
    return rowsAboutToBeRemovedRecord != rowsRemovedRecord;
  }

  /*! \brief Enables introspection of signals for rows removals for a QAbstractItemModel based model
   *
   * \code
   * MyModel model;
   * populateModel(model, {{1,"A"},{2,"B"}});
   *
   * RemoveRowsSignalsSpy spy(model);
   *
   * removeLastRowFromModel(model);
   *
   * REQUIRE( spy.rowsAboutToBeRemovedCount() == 1 );
   * REQUIRE( spy.rowsRemovedCount() == 1 );
   *
   * const auto rowsRemoved = spy.rowsRemovedAt(0);
   * REQUIRE( spy.rowsAboutToBeRemovedAt(0) == rowsRemoved );
   * REQUIRE( !rowsRemoved.parentIndex().isValid() );
   * REQUIRE( rowsRemoved.first() == 1 );
   * REQUIRE( rowsRemoved.last() == 1 );
   * \endcode
   *
   */
  class MDT_ITEMMODEL_TESTLIB_EXPORT RemoveRowsSignalsSpy : public QObject
  {
   Q_OBJECT

   public:

    /*! \brief Constructor
     */
    explicit RemoveRowsSignalsSpy(QAbstractItemModel & model) noexcept;

    /*! \brief Get the count of rowsAboutToBeRemoved signals this spy received
     */
    size_t rowsAboutToBeRemovedCount() const noexcept
    {
      return mRowsAboutToBeRemovedList.size();
    }

    /*! \brief Get the count of rowsRemoved signals this spy received
     */
    size_t rowsRemovedCount() const noexcept
    {
      return mRowsRemovedList.size();
    }

    /*! \brief Get the rowsAboutToBeRemoved signal record at given index
     *
     * \pre \a index must be in valid range ( \a index < rowsAboutToBeRemovedCount() )
     */
    const RowsAboutToBeRemovedSignalRecord & rowsAboutToBeRemovedAt(size_t index) const noexcept
    {
      assert( index < rowsAboutToBeRemovedCount() );

      return mRowsAboutToBeRemovedList[index];
    }

    /*! \brief Get the rowsRemoved signal record at given index
     *
     * \pre \a index must be in valid range ( \a index < rowsRemovedCount() )
     */
    const RowsRemovedSignalRecord & rowsRemovedAt(size_t index) const noexcept
    {
      assert( index < rowsRemovedCount() );

      return mRowsRemovedList[index];
    }

   private slots:

    void addRowsAboutToBeRemovedRecord(const QModelIndex & parent, int first, int last) noexcept;
    void addRowsRemovedRecord(const QModelIndex & parent, int first, int last) noexcept;

   private:

    std::vector<RowsAboutToBeRemovedSignalRecord> mRowsAboutToBeRemovedList;
    std::vector<RowsRemovedSignalRecord> mRowsRemovedList;
  };

}}} // namespace Mdt{ namespace ItemModel{ namespace TestLib{

#endif // #ifndef MDT_ITEM_MODEL_TEST_LIB_REMOVE_ROWS_SIGNALS_SPY_H
