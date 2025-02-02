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

#include <Mdt/Numeric/Limits.h>
#include <Mdt/Numeric/BasicConversion.h>
#include <cassert>

namespace Mdt{ namespace ItemModel{

  /*! \brief Adapter to use STL contiguous containers with Qt item models
   *
   * In Qt model/view, row acces is int index based.
   * In the STL containers, index is std::size_t or iterator based.
   *
   * \todo Discuss default constructed:
   * - Should it exist in adapter ? Yes
   * - Should it be imposed ?
   * - For the shared version, should it instanciate an empty container, or be a nullptr ??
   *   For the nullptr version, noexcept + not imposes default constructible + no CPU waste
   *   But, should be able to construct the container on insert (?)
   *
   * \todo What about read only containers ?
   * We should not impose to be mutable.
   *
   * \todo What about resizable containers ?
   * We should not impose to provide insert / erase
   *
   * \note The name StlContiguousContainerAdapter requires all the std::vector functions ?
   *
   * Note this:
   * \code
   * bool insertXXX(...)
   * {
   *   if constexpr(CanInsert){ // OR SupportsInsert - To be coherent with AbstractTableModel
   *     mContainer.insert(...);
   *     return true;
   *   }
   *   return false;
   * }
   * \endcode
   *
   * \todo Put usage example
   *
   * \section Mdt_ItemModel_StlContiguousContainerAdapter_ReadOnlyContainer Read only container
   *
   * Imagine we implement a serial port settings editor.
   * One of the settings is the interface.
   * Some serial port adapters supports not only RS-232, but also RS-485 and others.
   * \sa https://www.moxa.com/en/products/industrial-edge-connectivity/usb-to-serial-converters-usb-hubs/usb-to-serial-converters/uport-1200-1400-1600-series
   *
   * |Parameter value| Interface |
   * |:-------------:|:---------:|
   * |  0x00         | RS-232    |
   * |  0x01         | RS-485 2W |
   * |  0x02         | RS-422    |
   * |  0x03         | RS-485 4W |
   *
   * We present the list of available interfaces in a QComboBox.
   * The user see the names, like RS-232 .
   *
   * When we get the setting from the system, or from a preset file,
   * we also have to set the index in the combobox that represents the parameter value.
   *
   * \code
   * class InterfaceList
   * {
   *  public:
   *
   *   using size_type = std::vector<Interface>::size_type;
   *   using const_iterator = std::vector<Interface>::const_iterator;
   *
   *   // By default, only 1 interface is available: RS-232
   *   explicit
   *   InterfaceList();
   *
   *   size_type size() const noexcept;
   *
   *   const Interface & interfaceAt(size_type index) const noexcept;
   *
   *   const_iterator findPositionOfParameterValue(unsigned int value) const noexcept;
   *
   *  private:
   *
   *   std::vector<Interface> mList;
   * };
   * \endcode
   *
   * In this example, we choose to have our own API.
   * To conform to the StlContiguousContainerAdapter requirements,
   * we make a separate adapter. This reduces coupling.
   * We also could have choose to adapt or add reuired methods to be directly usable with the StlContiguousContainerAdapter.
   *
   * \todo remind: find() uses iterators.
   * If we want this, we have to provide iterators in the domain object.
   *
   * \code
   * class InterfaceListTableModelAdapter
   * {
   *  public:
   *
   *   using size_type = InterfaceList::size_type;
   *   using const_iterator = InterfaceList::const_iterator;
   *   using const_reference = const Interface &;
   *
   *   InterfaceListTableModelAdapter() = default;
   *
   *   size_type size() const noexcept
   *   {
   *     return mList.size();
   *   };
   *
   *   const Interface & at(size_type index) const noexcept
   *   {
   *     return mList.interfaceAt(index);
   *   }
   *
   *   findXXXX() ?????
   *
   *  private:
   *
   *   InterfaceList mList;
   * };
   * \endcode
   *
   * \code
   * class InterfaceListTableModel : QAbstractTableModel
   * {
   *  public:
   *
   *   // Constructor and othet methods omitted here
   *
   *   int rowCount( const QModelIndex &parent = QModelIndex() ) const
   *   {
   *     // parent checking omitted here
   *     return mList.rowCount();
   *   }
   *
   *   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
   *   {
   *     // index + role checking omitted here
   *     switch( index.column() ){
   *       case 0:
   *         return mList.atRow( index.row() ).name();
   *       case 1:
   *         return mList.atRow( index.row() ).parameterValue(); // HEX formatting omitted here
   *     }
   *
   *     return QVariant();
   *   }
   *
   *   int findRowOfParameterValue(unsigned int value) const noexcept
   *   {
   *     // Solution 1
   *     // Domain container provides a find() method and exposes const_iterator anyway
   *     const auto it = mList.container().findPositionOfParameterValue(value);
   *     return mList.rowFromPosition(it);
   *
   *     // Solution 2
   *     // Domain container provides an index based find() method
   *     const auto index = mList.container().findIndexOfParameterValue(value);
   *     return mList.rowFromIndex(index);
   *
   *     // Solution 3
   *     // Domain container does not provide a find() method - discouraged
   *     // Domain container has to expose const_iterator, cbegin() and cend()
   *     const auto pred = [value](const Interface & interface) -> bool
   *     {
   *       return interface.parameterValue() == value;
   *     };
   *     return mList.findRowOf(pred);
   *
   *     // Solution 4 - const_iterator based
   *     return mList.findRowOf(&InterfaceList::findPositionOfParameterValue(), value);
   *
   *     // Solution 5 - index based
   *     return mList.findRowOf(&InterfaceList::findIndexOfParameterValue(), value);
   *   }
   *
   *  private:
   *
   *   Mdt::ItemModel::StlContiguousContainerAdapter<InterfaceListTableModelAdapter> mList;
   * };
   * \endcode
   *
   * \todo put findRowOfParameterValue() solution 3-5 to the exploratory / rationale section
   *
   * Notice that in the %data() method, we don't have to care about int to size_type conversion.
   * This is done by StlContiguousContainerAdapter.
   *
   * \code
   * class MyTableModel : QAbstractTableModel
   * {
   *  public:
   *
   *
   *  private:
   *
   *   StlContiguousContainerAdapter<MyContainer> mContainer;
   * };
   * \endcode
   *
   * There are cases where we want to present a collection of elements,
   * based on a STL container, like std::vector,
   * by providing a Qt item model based access model.
   *
   * \note This adapter owns the underlaying container.
   * If the container should be shared, use SharedStlContiguousContainerAdapter.
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
   * \sa SharedStlContiguousContainerAdapter
   * \sa https://doc.qt.io/qt-6/qabstractitemmodel.html
   * \sa https://doc.qt.io/qt-6/qmodelindex.html
   * \sa https://en.cppreference.com/w/cpp/named_req/ContiguousContainer
   */
  template<typename Container>
  class StlContiguousContainerAdapter
  {
   public:

    using size_type = typename Container::size_type;
    using const_reference = typename Container::const_reference;

    /*! \brief Construct an empty container
     */
    explicit
    StlContiguousContainerAdapter() noexcept = default;

    /*! \brief Get the count of elements
     */
    int size() const noexcept
    {
      assert( Mdt::Numeric::int_canHoldValueOf_size_t( mContainer.size() ) );

      return Mdt::Numeric::int_from_size_t( mContainer.size() );
    }

    // bool isEmpty()

    /*! \brief
     *
     * \todo preconditions
     */
    const_reference at(int index) const
    {
    }

   private:

    Container mContainer;
  };

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_STL_CONTIGUOUS_CONTAINER_ADAPTER_H
