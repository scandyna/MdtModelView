// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_STL_CONTIGUOUS_CONTAINER_ADAPTER_H
#define MDT_ITEM_MODEL_STL_CONTIGUOUS_CONTAINER_ADAPTER_H

// #include "mdt_itemmodel_export.h"

namespace Mdt{ namespace ItemModel{

  /*! \brief Adapter to use STL contiguous containers with Qt item models
   *
   * \todo STL adapters holds the container. This is more like a view. No! View are read only
   * For lifetime issues, we should reference the container with a shared_ptr
   * We should be able to construct autonome ou passer un existant.
   *
   * In Qt model/view, row acces is int index based.
   * In the STL containers, index is std::size_t or iterator based.
   *
   * \todo Put usage example
   *
   * \code
   * class MyTableModel : QAbstractTableModel
   * {
   *  public:
   *
   *   void setContainer(shared_ptr<MyContainer> container)
   *   {
   *     mContainer = ?
   *   }
   *
   *  private:
   *
   *   StlContiguousContainerAdapter<MyContainer> mContainer;
   *   // OR
   *   StlContiguousContainerAdapter< std::shared_ptr<MyContainer> > mContainer;
   *   // Will not work, bacause shared_ptr is the wrong interface
   *   // OR
   *   std::shared_ptr< StlContiguousContainerAdapter<MyContainer> > mContainer
   * };
   * \endcode
   *
   * \code
   * class SharedStlContiguousContainerAdapter
   * {
   *  public:
   *
   *   setContainer(shared_ptr<MyContainer> container)
   *   {
   *     mContainer = container;
   *   }
   *
   *  private:
   *
   *   shared_ptr<Container> mContainer;
   * };
   * \endcode
   *
   * There are cases where we want to present a collection of elements,
   * based on a STL container, like std::vector,
   * by providing a Qt item model based access model.
   *
   * The first problem is the conversion between the container size type
   * and the int based indexing.
   *
   * \code
   * int rowCount( const QModelIndex &parent = QModelIndex() ) const
   * {
   *   // Here we should first be sure the container size can be safely converted to int
   *   // If ok, we should do some explicit cast
   * }
   * \endcode
   *
   * \code
   * QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
   * {
   *   // First, be shure that index.row() is >= 0
   *   // Also be sure the container size can be safely converted to int
   *   // If ok, we should do some explicit cast
   * }
   * \endcode
   *
   * To make sure we can convert STL container's indices to int,
   * we can limit the count of rows the model can present
   * (to a size far bigger than what makes sense to present to the user anyway).
   *
   * Sometimes we have to find the row that contains a certain element.
   * This is useful f.ex. to set the current index of a QComboBox.
   * While large collections should implement some index mapping,
   * doing a simple search is probably ok for small collections.
   *
   * \code
   * int findRowOfValue(const Value & value) const
   * {
   *   // We should use STL algorithm and iterators here
   *   // We can use std::distance() to convert to an index (a row here)
   *   // The returned index is something like long int,
   *   // we have to be sure it can be safely converted to int.
   *   // If ok, we should do some explicit cast
   * }
   * \endcode
   *
   * Resizing the collection is also some problem to tackle.
   *
   * \code
   * bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex())
   * {
   *   // Check row >= 0, count >= 1
   *   // Cast row and count to appropriate STL types
   *   // Check that the combination of (STL type casted) row and count can be represented by the int indexed model
   *   // Create an iterator using std::next()
   *   // Call Container::insert()
   * }
   * \endcode
   *
   * \sa insertToStlContainer()'s implementation.
   *
   * \note To deal with beginInsertRows(), endInsertRows(), etc...,
   * is out of scope of this adapter.
   * See AbstractTableModel for that.
   *
   * \sa https://doc.qt.io/qt-6/qabstractitemmodel.html
   * \sa https://doc.qt.io/qt-6/qmodelindex.html
   * \sa https://en.cppreference.com/w/cpp/named_req/ContiguousContainer
   */
  class StlContiguousContainerAdapter
  {
   public:

    

   private:

    
  };

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_STL_CONTIGUOUS_CONTAINER_ADAPTER_H
