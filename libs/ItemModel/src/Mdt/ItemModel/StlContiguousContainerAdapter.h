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

#include "Mdt/TypeTraits/StlContainerHelpers.h"
#include <Mdt/Numeric/Limits.h>
#include <Mdt/Numeric/BasicConversion.h>
#include <type_traits>
#include <utility>
#include <cassert>

namespace Mdt{ namespace ItemModel{

  /*! \brief Adapter to use STL style containers with Qt item models
   *
   *
   * This adapter can be used with containers that are domain specific,
   * and that do not provide all the STL required interface.
   *
   * Goal is to avoid having to adapt the container itself
   * to be usable with an item model.
   *
   * \todo fix noexcept
   *
   * \tparam Container
   * \tparam SizeType
   *
   * \tparam ValueType STL value_type provided by the container.
   * This is a required type, void is not allowed.
   * const_reference is also deduced from ValueType.
   *
   * \tparam ConstIterator STL const_iterator provided by the container.
   * Can be void if the container not provides const_iterator.
   * If the container provides const_iterator,
   * it also must provide cbegin() and cend().
   *
   * \tparam Reference STL reference provided by the container.
   *
   *
   * \tparam FunctionMap
   *
   * \sa Mdt::ItemModel::AbstractTableModel
   * \sa https://doc.qt.io/qt-6/qabstractitemmodel.html
   * \sa https://doc.qt.io/qt-6/qmodelindex.html
   */

  /*! \brief Adapter to use STL contiguous containers with Qt item models
   *
   * In Qt model/view, row acces is int index based.
   * In the STL containers, index is std::size_t or iterator based.
   *
   * A goal of this adapter is to help to convert between those types
   * in some checked way (contract programming based).
   *
   * To be usable with domain specific containers,
   * this adapter requires a minimal subset of the STL requirements.
   *
   * To avoid having to modify domain specific containers,
   * a \a FunctionMap can be provided,
   * to map the domain specific container's methods to this adapter.
   *
   * \section Mdt_ItemModel_StlContiguousContainerAdapter_MinimalReadOnlyContainer A minimal read only container example
   *
   * Example of a simple read only container:
   * \code
   * class ReadOnlyList
   * {
   *  public:
   *
   *   using size_type = std::vector<Item>::size_type;
   *
   *   size_type getSizeCustom() const noexcept;
   *   const Item & itemAt(size_type index) const noexcept;
   * };
   * \endcode
   *
   * This domain container exposes \a size_type in a STL standard way.
   * It also provides domain specific methods.
   * To be usable with this adapter,
   * we have to provide a function map:
   * \code
   * struct ReadOnlyListTableModelAdapterFunctionMap
   * {
   *   using size_type = ReadOnlyList::size_type;
   *   using const_reference = const Item &;
   *
   *   static
   *   size_type size(const ReadOnlyList & list) noexcept
   *   {
   *     return list.getSizeCustom();
   *   }
   *
   *   static
   *   const_reference atIndex(const ReadOnlyList & list, size_type index) noexcept
   *   {
   *     return list.itemAt(index);
   *   }
   * };
   * \endcode
   *
   * A function map provides a set of STL types, and also static methods.
   * The first argument of those methods is the container.
   * This choice was made to avoid referencing the container in the function map,
   * to prevent life time issues.
   *
   * Now we can implement the table model:
   * \code
   * class ReadOnlyListTableModel : QAbstractTableModel
   * {
   *  public:
   *
   *   ReadOnlyListTableModel(const ReadOnlyList & list, QObject *parent = nullptr)
   *    : QAbstractTableModel(parent),
   *      mList(list)
   *   {
   *   }
   *
   *   int rowCount( const QModelIndex &parent = QModelIndex() ) const override
   *   {
   *     // parent checking omitted here
   *     return mList.rowCount();
   *   }
   *
   *   int columnCount( const QModelIndex &parent = QModelIndex() ) const override
   *   {
   *     // parent checking omitted here
   *     return 2;
   *   }
   *
   *   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
   *   {
   *     // index + role checking omitted here
   *     switch( index.column() ){
   *       case 0:
   *         return mList.atRow( index.row() ).id();
   *       case 1:
   *         return mList.atRow( index.row() ).name();
   *     }
   *
   *     return QVariant();
   *   }
   *
   *  private:
   *
   *   Mdt::ItemModel::StlContiguousContainerAdapter<ReadOnlyList, ReadOnlyListTableModelAdapterFunctionMap> mList;
   * };
   * \endcode
   *
   * We could also implement our model using Mdt::ItemModel::AbstractTableModel:
   * \code
   * class ReadOnlyListTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  public:
   *
   *   ReadOnlyListTableModel(const ReadOnlyList & list, QObject *parent = nullptr)
   *    : AbstractTableModel(parent),
   *      mList(list)
   *   {
   *   }
   *
   *  private:
   *
   *   int rowCountWithoutParentIndex() const noexcept override
   *   {
   *     return mList.rowCount();
   *   }
   *
   *   int columnCountWithoutParentIndex() const noexcept override
   *   {
   *     return 2;
   *   }
   *
   *   QVariant displayRoleData(const QModelIndex & index) const noexcept override
   *   {
   *     assert( indexIsValidAndInRange(index) );
   *
   *     switch( index.column() ){
   *       case 0:
   *         return mList.atRow( index.row() ).id();
   *       case 1:
   *         return mList.atRow( index.row() ).name();
   *     }
   *
   *     return QVariant();
   *   }
   *
   *   Mdt::ItemModel::StlContiguousContainerAdapter<ReadOnlyList, ReadOnlyListTableModelAdapterFunctionMap> mList;
   * };
   * \endcode
   *
   * \section Mdt_ItemModel_StlContiguousContainerAdapter_MutableContainer Use setData() with a mutable container
   *
   * While it's probably ok to return a const reference to an element,
   * for mutation this can be less desirable.
   * The domain container probably have to limit the access to some attributes,
   * and maybe also do some work to keep invariance.
   *
   * Another approach is to expose some raw data to the editor,
   * then validate the inputs and finally build a domain object.
   *
   * Note that a mutable container also has to provide read access
   * to be usable in a (table/item) model.
   *
   * \subsection Mdt_ItemModel_StlContiguousContainerAdapter_MutableContainer_DirectAccess Direct access to a mutable element
   *
   * Example of a list that provides a access to an element for mutation:
   * \code
   * class MutableListRawData
   * {
   *  public:
   *
   *   using size_type = std::vector<Item>::size_type;
   *
   *   size_type getSizeCustom() const noexcept;
   *   const Item & itemAt(size_type index) const noexcept;
   *
   *   Item & mutableItemAt(size_type index) noexcept;
   * };
   * \endcode
   *
   * As for the read only example, we implement a function map:
   * \code
   * struct MutableListRawDataTableModelAdapterFunctionMap
   * {
   *   using size_type = MutableListRawData::size_type;
   *   using const_reference = const Item &;
   *   using reference = Item &;
   *
   *   static
   *   size_type size(const MutableListRawData & list) noexcept
   *   {
   *     return list.getSizeCustom();
   *   }
   *
   *   static
   *   const_reference atIndex(const MutableListRawData & list, size_type index) noexcept
   *   {
   *     return list.itemAt(index);
   *   }
   *
   *   static
   *   reference atIndexMutable(MutableListRawData & list, size_type index) noexcept
   *   {
   *     return list.mutableItemAt(index);
   *   }
   * };
   * \endcode
   *
   * The function map is similar to the read only example,
   * but adds the \a reference type and the \a atIndexMutable() function.
   *
   * Here is an example for the mutation part of a table model:
   * \code
   * class MutableListRawDataTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  public:
   *
   *   // Constructor omitted
   *
   *   Qt::ItemFlags flags(const QModelIndex & index) const override
   *   {
   *     if( !indexIsValidAndInRange(index) ){
   *       return AbstractTableModel::flags(index);
   *     }
   *     if( index.column() == 1 ){
   *       return AbstractTableModel::flags(index) | Qt::ItemIsEditable;
   *     }
   *
   *     return AbstractTableModel::flags(index);
   *   }
   *
   *  private:
   *
   *   // Methods identical to the read only example omitted here
   *
   *   bool setEditRoleData(const QModelIndex & index, const QVariant & value) override
   *   {
   *     assert( indexIsValidAndInRange(index) );
   *
   *     switch( index.column() ){
   *       case 1:
   *         mList.atRowMutable( index.row() ).setName( value.toString() );
   *         return true;
   *     }
   *
   *     return false;
   *   }
   *
   *   Mdt::ItemModel::StlContiguousContainerAdapter<MutableListRawData, MutableListRawDataTableModelAdapterFunctionMap> mList;
   * };
   * \endcode
   *
   * \subsection Mdt_ItemModel_StlContiguousContainerAdapter_MutableContainer_UseDomainMethods Use domain methods for mutation
   *
   * Example of a list that provides domain methods to update some attributes (not all):
   * \code
   * class MutableList
   * {
   *  public:
   *
   *   using size_type = std::vector<Item>::size_type;
   *
   *   size_type getSizeCustom() const noexcept;
   *   const Item & itemAt(size_type index) const noexcept;
   *
   *   void setNameAt(size_type index, const QString & name);
   * };
   * \endcode
   *
   * As above, we implement a function map:
   * \code
   * struct MutableListTableModelAdapterFunctionMap
   * {
   *   using size_type = MutableList::size_type;
   *   using const_reference = const Item &;
   *
   *   static
   *   size_type size(const MutableList & list) noexcept
   *   {
   *     return list.getSizeCustom();
   *   }
   *
   *   static
   *   const_reference atIndex(const MutableList & list, size_type index) noexcept
   *   {
   *     return list.itemAt(index);
   *   }
   *
   * };
   * \endcode
   *
   * Notice that we not provide \a reference and \a atRowMutable() anymore.
   *
   * Here is an example for the mutation part of a table model:
   * \code
   * class MutableListTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  public:
   *
   *   // Constructor omitted
   *
   *   // flags() method omitted
   *
   *  private:
   *
   *   // Methods identical to the read only example omitted here
   *
   *   bool setEditRoleData(const QModelIndex & index, const QVariant & value) override
   *   {
   *     assert( indexIsValidAndInRange(index) );
   *
   *     const size_type containerIndex = mList.indexFromRow( index.row() );
   *
   *     switch( index.column() ){
   *       case 1:
   *         mList.containerMutable().setNameAt( containerIndex, value.toString() );
   *         return true;
   *     }
   *
   *     return false;
   *   }
   *
   *   Mdt::ItemModel::StlContiguousContainerAdapter<MutableList, MutableListTableModelAdapterFunctionMap> mList;
   * };
   * \endcode
   *
   * \section Mdt_ItemModel_StlContiguousContainerAdapter_ResizableContainers Resizable container examples
   *
   * Some containers supports inserting elements at any place.
   * This maps to Qt model insertRows().
   *
   * Some containers only provides methods like push_back().
   *
   * \subsection Mdt_ItemModel_StlContiguousContainerAdapter_ResizableContainers_Insert Container that provides insert()
   *
   * Example of a container that provides insert:
   * \code
   * class ListWithInsert
   * {
   *  public:
   *
   *   using size_type = std::vector<Item>::size_type;
   *   using const_iterator = std::vector<Item>::const_iterator;
   *
   *   size_type getSizeCustom() const noexcept;
   *   const Item & itemAt(size_type index) const noexcept;
   *
   *   void insert(const_iterator pos, size_type count, const Item & item);
   * };
   * \endcode
   *
   * Here is the implementation of the function map:
   * \code
   * struct ListWithInsertTableModelAdapterFunctionMap
   * {
   *   using size_type = ListWithInsert::size_type;
   *   using const_reference = const Item &;
   *   using const_iterator = ListWithInsert::const_iterator;
   *
   *   static
   *   size_type size(const ListWithInsert & list) noexcept
   *   {
   *     return list.getSizeCustom();
   *   }
   *
   *   static
   *   const_reference atIndex(const ListWithInsert & list, size_type index) noexcept
   *   {
   *     return list.itemAt(index);
   *   }
   *
   *   static
   *   void insert(ListWithInsert & list, const_iterator pos, size_type count, const_reference item)
   *   {
   *     list.insert(pos, count, item);
   *   }
   * };
   * \endcode
   *
   * Here is an example for the insert part of a table model:
   * \code
   * class ListWithInsertTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  public:
   *
   *   // Constructor omitted
   *
   *  private:
   *
   *   // Methods identical to the read only example omitted here
   *
   *   bool doSupportsInsertRows() const noexcept override
   *   {
   *     return true;
   *   }
   *
   *   void doInsertRows(int row, int count) override
   *   {
   *     assert( rowAndCountIsValidForInsertRows(row, count) );
   *
   *     mList.insertRows( row, count, Item() );
   *   }
   *
   *   Mdt::ItemModel::StlContiguousContainerAdapter<ListWithInsert, ListWithInsertTableModelAdapterFunctionMap> mList;
   * };
   * \endcode
   *
   * \subsection Mdt_ItemModel_StlContiguousContainerAdapter_ResizableContainers_PushBack Container that provides push_back() or similar
   *
   * Example of a container that provides append:
   * \code
   * class ListWithAppend
   * {
   *  public:
   *
   *   using size_type = std::vector<Item>::size_type;
   *
   *   size_type getSizeCustom() const noexcept;
   *   const Item & itemAt(size_type index) const noexcept;
   *
   *   void append(const Item & item) noexcept;
   * };
   * \endcode
   *
   * Here is the implementation of the function map:
   * \code
   * struct ListWithAppendTableModelAdapterFunctionMap
   * {
   *   using size_type = ListWithAppend::size_type;
   *   using const_reference = const Item &;
   *
   *   static
   *   size_type size(const ListWithAppend & list) noexcept
   *   {
   *     return list.getSizeCustom();
   *   }
   *
   *   static
   *   const_reference atIndex(const ListWithAppend & list, size_type index) noexcept
   *   {
   *     return list.itemAt(index);
   *   }
   *
   *   static
   *   void push_back(ListWithAppend & list, const_reference item) noexcept
   *   {
   *     list.append(item);
   *   }
   * };
   * \endcode
   *
   * Here is an example for the append part of a table model:
   * \code
   * class ListWithAppendTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  public:
   *
   *   // Constructor omitted
   *
   *  private:
   *
   *   // Methods identical to the read only example omitted here
   *
   *   bool doSupportsAppendRow() const noexcept override
   *   {
   *     return true;
   *   }
   *
   *   void doAppendRow() override
   *   {
   *     mList.appendRow( Item() );
   *   }
   *
   *   Mdt::ItemModel::StlContiguousContainerAdapter<ListWithAppend, ListWithAppendTableModelAdapterFunctionMap> mList;
   * };
   * \endcode
   *
   * \subsection Mdt_ItemModel_StlContiguousContainerAdapter_ResizableContainers_Erase Remove elements with erase()
   *
   * Example of a container that provides erase:
   * \code
   * class ListWithErase
   * {
   *  public:
   *
   *   using size_type = std::vector<Item>::size_type;
   *   using difference_type = std::vector<Item>::difference_type;
   *   using const_iterator = std::vector<Item>::const_iterator;
   *
   *   size_type getSizeCustom() const noexcept;
   *   const Item & itemAt(size_type index) const noexcept;
   *
   *   void erase(const_iterator first, const_iterator last);
   *
   *   const_iterator cbegin() const noexcept;
   *   const_iterator cend() const noexcept;
   * };
   * \endcode
   *
   * Here is the implementation of the function map:
   * \code
   * struct ListWithEraseTableModelAdapterFunctionMap
   * {
   *   using size_type = ListWithErase::size_type;
   *   using const_reference = const Item &;
   *   using difference_type = ListWithErase::difference_type;
   *   using const_iterator = ListWithErase::const_iterator;
   *
   *   static
   *   size_type size(const ListWithErase & list) noexcept
   *   {
   *     return list.getSizeCustom();
   *   }
   *
   *   static
   *   const_reference atIndex(const ListWithErase & list, size_type index) noexcept
   *   {
   *     return list.itemAt(index);
   *   }
   *
   *   static
   *   void erase(ListWithErase & list, const_iterator first, const_iterator last) noexcept
   *   {
   *     list.erase(first, last);
   *   }
   *
   *   static
   *   const_iterator begin(const ListWithErase & list) noexcept
   *   {
   *     return list.cbegin();
   *   }
   * };
   * \endcode
   *
   * Here is an example for the remove rows part of a table model:
   * \code
   * class ListWithEraseTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  public:
   *
   *   // Constructor omitted
   *
   *  private:
   *
   *   // Methods identical to the read only example omitted here
   *
   *   bool doSupportsRemoveRows() const noexcept override
   *   {
   *     return true;
   *   }
   *
   *   void doRemoveRows(int row, int count) override
   *   {
   *     assert( rowAndCountIsValidForRemoveRows(row, count) );
   *
   *     mList.removeRows(row, count);
   *   }
   *
   *   Mdt::ItemModel::StlContiguousContainerAdapter<ListWithErase, ListWithEraseTableModelAdapterFunctionMap> mList;
   * };
   * \endcode
   *
   * \subsection Mdt_ItemModel_StlContiguousContainerAdapter_ResizableContainers_RemoveAt Remove an element at a given index
   *
   * \todo Document + implement or remove
   *
   * \todo rowFromIndex()
   * \todo indexFromRow()
   *
   * \todo Document the following examples with AbstractTableModel
   *
   * \todo document minimal requirements on the container.
   * For types, size_type and const_reference .
   *
   * \todo example with a container that provides all requirements, like std::vector, or one with less than vector, but all for this adapter
   *
   * \todo example with vector: fast way to implement container for tests
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
   * \section Mdt_ItemModel_StlContiguousContainerAdapter_MoreRealReadOnlyContainer A more real read only container example
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
   *   // Constructor and other methods omitted here
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
   *
   * \section Mdt_ItemModel_StlContiguousContainerAdapter_Rationale Rationale
   *
   */
  template<typename Container, typename FunctionMap>
  class StlContiguousContainerAdapter
  {
   public:

    /*! \brief STL size_type
     *
     * Will be FunctionMap::size_type
     *
     * size_type is mandatory
     */
    using size_type = typename FunctionMap::size_type;

    /*! \brief STL difference_type
     *
     * Will be FunctionMap::difference_type if \a FunctionMap defines it,
     * otherwise void.
     */
    using difference_type = Mdt::TypeTraits::member_difference_type_or_void<FunctionMap>;

    /*! \brief STL const_reference
     *
     * Will be FunctionMap::const_reference
     *
     * const_reference is mandatory
     */
    using const_reference = typename FunctionMap::const_reference;

    /*! \brief STL reference
     *
     * Will be FunctionMap::reference if \a FunctionMap defines it,
     * otherwise void.
     */
    using reference = Mdt::TypeTraits::member_reference_or_void<FunctionMap>;

    /*! \brief STL const_iterator
     *
     * Will be FunctionMap::const_iterator if \a FunctionMap defines it,
     * otherwise void.
     */
    using const_iterator = Mdt::TypeTraits::member_const_iterator_or_void<FunctionMap>;

    /*! \brief Construct an adapter with a default constructed container
     */
    explicit
    StlContiguousContainerAdapter() noexcept(std::is_nothrow_default_constructible_v<Container>) = default;

    /*! \brief Construct an adapter with a copy of given container
     */
    explicit
    StlContiguousContainerAdapter(const Container & container) noexcept(std::is_nothrow_copy_constructible_v<Container>)
     : mContainer(container)
    {
    }

    /*! \brief Construct an adapter with given container moved in
     */
    explicit
    StlContiguousContainerAdapter(Container && container) noexcept(std::is_nothrow_move_constructible_v<Container>)
     : mContainer( std::move(container) )
    {
    }

    /*! \brief Get the count of rows for the model
     *
     * \pre The current size of the container must be convertible to int
     */
    int rowCount() const
    {
      assert( Mdt::Numeric::int_canHoldValueOf_T( FunctionMap::size(mContainer) ) );

      return Mdt::Numeric::int_from_T( FunctionMap::size(mContainer) );
    }

    /*! \brief Get the element at given row
     *
     * \pre \a row must be in range of the container ( 0 >= \a row < rowCount() )
     */
    const_reference atRow(int row) const
    {
      assert( row >= 0 );
      assert( row < rowCount() );

      return FunctionMap::atIndex( mContainer, indexFromRow(row) );
    }

    /*! \brief Access the element at given row for mutation
     *
     * \pre the reference type must be valid.
     * \pre \a row must be in range ( 0 >= \a row < rowCount() )
     */
    reference atRowMutable(int row) noexcept
    {
      static_assert( !std::is_void_v<reference>, "call StlContiguousContainerAdapter::atRowMutable() requires FunctionMap::reference to be defined" );
      assert( row >= 0 );
      assert( row < rowCount() );

      return FunctionMap::atIndexMutable( mContainer, indexFromRow(row) );
    }

    /// \todo For return reference (can be void), can auto help ?

    /*! \brief Inserts count rows into the container before the given row
     *
     * \todo precondition: the container must be able to store row + count
     *
     * \todo static preconditions like in test
     *
     * \todo should return void
     *
     * To use this method, the function map must have an insert function of this form:
     * \code
     * static
     * void insert(Container & container, const_iterator pos, size_type count, const_reference value);
     * \endcode
     *
     * \pre \a row must be >= 0
     * \pre \a row must be <= rowCount()
     * \pre \a count must be >= 1
     */
    bool insertRows(int row, int count, const_reference value)
    {
      return false;
    }

    /*! \brief Append an element
     *
     * \todo precondition: the container must be able to store another element
     *
     * To use this method, the function map must have a push_back function of this form:
     * \code
     * static
     * void push_back(Container & container, const_reference value);
     * \endcode
     */
    void appendRow(const_reference value)
    {
      FunctionMap::push_back(mContainer, value);
    }

    /*! \brief Removes count rows starting with the given row
     *
     * To use this method, the function map must have an erase function of this form:
     * \code
     * static
     * void erase(Container & container, const_iterator first, const_iterator last);
     * \endcode
     *
     * To define first and last, a const-qualified begin is also required:
     * \code
     * static
     * const_iterator begin(const Container & container) const noexcept;
     * \endcode
     *
     * \pre FunctionMap::difference_type must be defined
     * \pre FunctionMap::const_iterator must be defined
     * \pre \a row must be >= 0
     * \pre \a count must be >= 1
     * \pre ( \a row + \a count ) must be <= rowCount()
     */
    void removeRows(int row, int count)
    {
      static_assert( !std::is_void_v<difference_type>, "call StlContiguousContainerAdapter::removeRows() requires FunctionMap::difference_type to be defined" );
      static_assert( !std::is_void_v<const_iterator>, "call StlContiguousContainerAdapter::removeRows() requires FunctionMap::const_iterator to be defined" );
      assert( row >= 0 );
      assert( count >= 1 );
      assert( (row + count) <= rowCount() );

    }

    /*! \brief Get the size_type index from given row
     *
     * \pre \a row must be in range of the container ( 0 >= \a row < rowCount() )
     */
    size_type indexFromRow(int row) const
    {
      assert( row >= 0 );
      assert( row < rowCount() );

      /*
       * row is in the range of the container,
       * it is also in the range of size_type
       */
      return static_cast<size_type>(row);
    }

    /*! \brief Access the container for mutation
     */
    Container & containerMutable() noexcept
    {
      return mContainer;
    }

   private:

    Container mContainer;
  };

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_STL_CONTIGUOUS_CONTAINER_ADAPTER_H
