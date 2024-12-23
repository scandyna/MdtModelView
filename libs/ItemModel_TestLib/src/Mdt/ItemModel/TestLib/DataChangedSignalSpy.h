// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_TEST_LIB_DATA_CHANGED_SIGNAL_SPY_H
#define MDT_ITEM_MODEL_TEST_LIB_DATA_CHANGED_SIGNAL_SPY_H

#include "mdt_itemmodel_testlib_export.h"
#include <QAbstractItemModel>
#include <QObject>
#include <QVector>
#include <QModelIndex>
#include <cstddef>
#include <vector>
#include <cassert>

namespace Mdt{ namespace ItemModel{ namespace TestLib{

  /*! \brief Record for DataChangedSignalSpy
   *
   * \sa DataChangedSignalSpy
   */
  class MDT_ITEMMODEL_TESTLIB_EXPORT DataChangedSignalRecord
  {
   public:

    /*! \brief Constructor
     */
    explicit DataChangedSignalRecord(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles) noexcept;

    /*! \brief Get the top left index
     */
    const QModelIndex & topLeftIndex() const noexcept
    {
      return mTopLeftIndex;
    }

    /*! \brief Get the bottom right index
     */
    const QModelIndex & bottomRightIndex() const noexcept
    {
      return mBottomRightIndex;
    }

    /*! \brief Get the roles
     */
    const QVector<int> & roles() const noexcept
    {
      return mRoles;
    }

   private:

    QModelIndex mTopLeftIndex;
    QModelIndex mBottomRightIndex;
    QVector<int> mRoles;
  };

  /*! \brief Enables introspection of the dataChanged() signal for a QAbstractItemModel based model
   *
   * \code
   * MyModel model;
   * populateModel(model, {{1,"A"},{2,"B"}});
   *
   * DataChangedSignalSpy spy(model);
   *
   * QModelIndex index = model.index(1, 0);
   * model.setData(index, "test data");
   *
   * REQUIRE( spy.count() == 1 );
   * REQUIRE( spy.firstTopLeftIndex() == index );
   * REQUIRE( spy.firstBottomRightIndex() == index );
   * REQUIRE( spy.firstRoles().count() == 0 );
   * \endcode
   *
   * Maybe the signal should be emitted more than once:
   * \code
   * REQUIRE( spy.count() == 2 );
   * REQUIRE( spy.at(0).topLeftIndex() == index1 );
   * ...
   * REQUIRE( spy.at(1).topLeftIndex() == index2 );
   * ...
   * \endcode
   */
  class MDT_ITEMMODEL_TESTLIB_EXPORT DataChangedSignalSpy : public QObject
  {
   Q_OBJECT

   public:

    /*! \brief Constructor
     */
    explicit DataChangedSignalSpy(QAbstractItemModel & model) noexcept;

    /*! \brief Get the count of signals this spy received
     */
    size_t count() const noexcept
    {
      return mList.size();
    }

    /*! \brief Get the signal record at given index
     *
     * \pre \a index must be in valid range ( \a index < count() )
     */
    const DataChangedSignalRecord & at(size_t index) const noexcept
    {
      assert( index < count() );

      return mList[index];
    }

    /*! \brief Get the top left index for the first signal record
     *
     * \pre This spy must have at least 1 record ( count() > 0 )
     */
    const QModelIndex & firstTopLeftIndex() const noexcept
    {
      assert( count() > 0 );

      return mList[0].topLeftIndex();
    }

    /*! \brief Get the bottom right index for the first signal record
     *
     * \pre This spy must have at least 1 record ( count() > 0 )
     */
    const QModelIndex & firstBottomRightIndex() const noexcept
    {
      assert( count() > 0 );

      return mList[0].bottomRightIndex();
    }

    /*! \brief Get the roles for the first signal record
     *
     * \pre This spy must have at least 1 record ( count() > 0 )
     */
    const QVector<int> & firstRoles() const noexcept
    {
      assert( count() > 0 );

      return mList[0].roles();
    }

   private slots:

    void addRecord(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles) noexcept;

   private:

    std::vector<DataChangedSignalRecord> mList;
  };

}}} // namespace Mdt{ namespace ItemModel{ namespace TestLib{

#endif // #ifndef MDT_ITEM_MODEL_TEST_LIB_DATA_CHANGED_SIGNAL_SPY_H
