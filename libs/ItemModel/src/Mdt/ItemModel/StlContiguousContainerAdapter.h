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
#include "Mdt/ItemModel/StlHelpers.h"
#include "Mdt/ItemModel/NumericLimits.h"
#include <Mdt/Numeric/Limits.h>
#include <Mdt/Numeric/BasicConversion.h>
#include <Mdt/Numeric/IntegerComparison.h>
#include <type_traits>
#include <iterator>
#include <utility>
#include <cassert>

namespace Mdt{ namespace ItemModel{

  /*! \brief Adapter to use STL contiguous containers with Qt item models
   *
   * \todo fix noexcept
   *
   * In Qt model/view, row acces is int index based.
   * In the STL containers, index is std::size_t or iterator based.
   *
   * A goal of this adapter is to help to convert between those types
   * in some checked way (contract programming based).
   *
   * This adapter can be used with containers that are domain specific,
   * and that do not provide all the STL required interface.
   *
   * To be usable with domain specific containers,
   * this adapter requires a minimal subset of the STL requirements.
   *
   * To avoid having to modify domain specific containers,
   * a \a FunctionMap can be provided,
   * to map the domain specific container's methods to this adapter.
   *
   * \note This adapter owns the underlaying container.
   * If the container should be shared, use SharedStlContiguousContainerAdapter.
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
   * Notice that we not provide \a reference and \a atIndexMutable() anymore.
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
   * This maps to Qt model %insertRows().
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
   *   using difference_type = std::vector<Item>::difference_type;
   *   using const_iterator = std::vector<Item>::const_iterator;
   *
   *   size_type getSizeCustom() const noexcept;
   *   const Item & itemAt(size_type index) const noexcept;
   *
   *   void insert(const_iterator pos, size_type count, const Item & item);
   *
   *   const_iterator cbegin() const noexcept;
   *   const_iterator cend() const noexcept;
   * };
   * \endcode
   *
   * Here is the implementation of the function map:
   * \code
   * struct ListWithInsertTableModelAdapterFunctionMap
   * {
   *   using size_type = ListWithInsert::size_type;
   *   using const_reference = const Item &;
   *   using difference_type = ListWithInsert::difference_type;
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
   *
   *   static
   *   const_iterator cbegin(const ListWithInsert & list) noexcept
   *   {
   *     return list.cbegin();
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
   * \subsection Mdt_ItemModel_StlContiguousContainerAdapter_ResizableContainers_InsertAndLimit Container that provides insert() and limits the count of elements
   *
   * Example of a container that provides insert and a limit of the allowed count of elements:
   * \code
   * class ListWithInsertAndLimit
   * {
   *  public:
   *
   *   using size_type = std::vector<Item>::size_type;
   *   using difference_type = std::vector<Item>::difference_type;
   *   using const_iterator = std::vector<Item>::const_iterator;
   *
   *   size_type getSizeCustom() const noexcept;
   *   size_type maximumElementCount() const noexcept;
   *   const Item & itemAt(size_type index) const noexcept;
   *
   *   void insert(const_iterator pos, size_type count, const Item & item);
   *
   *   const_iterator cbegin() const noexcept;
   *   const_iterator cend() const noexcept;
   * };
   * \endcode
   *
   * The function map is the same as above,
   * but it also provides the %maxSize() function:
   * \code
   * struct ListWithInsertAndLimitTableModelAdapterFunctionMap
   * {
   *   ...
   *
   *   static
   *   size_type maxSize(const ListWithInsertAndLimit & list)
   *   {
   *     return list.maximumElementCount();
   *   }
   *
   *   ...
   * }
   * \endcode
   *
   * Here is an example for the insert part of a table model:
   * \code
   * class ListWithInsertAndLimitTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  public:
   *
   *   // Constructor omitted
   *
   *  private:
   *
   *   // Methods identical to the read only example omitted here
   *
   *   int doMaxRowCount() const override
   *   {
   *     return mList.maxRowCount();
   *   }
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
   *   Mdt::ItemModel::StlContiguousContainerAdapter<ListWithInsertAndLimit, ListWithInsertAndLimitTableModelAdapterFunctionMap> mList;
   * };
   * \endcode
   *
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
   * \note Defining a maximum count of elements is also possible as explained above:
   * \ref Mdt_ItemModel_StlContiguousContainerAdapter_ResizableContainers_InsertAndLimit
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
   *   const_iterator cbegin(const ListWithErase & list) noexcept
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
   *
   * \section Mdt_ItemModel_StlContiguousContainerAdapter_UseStlConformContainer Use STL conform container
   *
   * If the container provides all the required types and methods for this adapter,
   * StlContiguousContainerFunctionMap can be used:
   * \code
   * class StdVectorTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  public:
   *
   *   using List = std::vector<Item>;
   *
   *   StdVectorTableModel(const List & list, QObject *parent = nullptr)
   *    : AbstractTableModel(parent),
   *      mList(list)
   *   {
   *   }
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
   *   int rowCountWithoutParentIndex() const override
   *   {
   *     return mList.rowCount();
   *   }
   *
   *   int columnCountWithoutParentIndex() const override
   *   {
   *     return 2;
   *   }
   *
   *   QVariant displayRoleData(const QModelIndex & index) const override
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
   *   bool doSupportsInsertRows() const noexcept override
   *   {
   *     return true;
   *   }
   *
   *   bool doSupportsRemoveRows() const noexcept override
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
   *   void doRemoveRows(int row, int count) override
   *   {
   *     assert( rowAndCountIsValidForRemoveRows(row, count) );
   *
   *     mList.removeRows(row, count);
   *   }
   *
   *   Mdt::ItemModel::StlContiguousContainerAdapter< List, Mdt::ItemModel::StlContiguousContainerFunctionMap<List> > mList;
   * };
   * \endcode
   *
   * The above example is almost a complete implementation of a memory table model (header data is missing).
   *
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
   *   using difference_type = std::vector<Interface>::difference_type;
   *   using const_iterator = std::vector<Interface>::const_iterator;
   *
   *   // By default, only 1 interface is available: RS-232
   *   explicit
   *   InterfaceList();
   *
   *   size_type interfaceCount() const noexcept;
   *
   *   const Interface & interfaceAt(size_type index) const noexcept;
   *
   *   const_iterator findPositionOfParameterValue(unsigned int value) const noexcept;
   *
   *   const_iterator cbegin() const noexcept;
   *   const_iterator cend() const noexcept;
   *
   *  private:
   *
   *   std::vector<Interface> mList;
   * };
   * \endcode
   *
   * In this example, we choose to have our own API.
   * We could have chosen to adapt or add required methods to be directly usable with the StlContiguousContainerAdapter.
   *
   * To reduce coupling, and conform to the StlContiguousContainerAdapter requirements,
   * we make a function map:
   * \code
   * struct InterfaceListTableModelAdapterFunctionMap
   * {
   *   using size_type = InterfaceList::size_type;
   *   using difference_type = InterfaceList::difference_type;
   *   using const_reference = const Interface &;
   *   using const_iterator = InterfaceList::const_iterator;
   *
   *   static
   *   size_type size(const InterfaceList & list) noexcept
   *   {
   *     return list.interfaceCount();
   *   }
   *
   *   static
   *   const_reference atIndex(const InterfaceList & list, size_type index) noexcept
   *   {
   *     return list.interfaceAt(index);
   *   }
   *
   *   static
   *   const_iterator cbegin(const InterfaceList & list) noexcept
   *   {
   *     return list.cbegin();
   *   }
   * };
   * \endcode
   * Note that cbegin() must be provided.
   *
   * Here is an example of a table model:
   * \code
   * class InterfaceListTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  public:
   *
   *   InterfaceListTableModel(const InterfaceList & list, QObject *parent = nullptr)
   *    : AbstractTableModel(parent),
   *      mList(list)
   *   {
   *   }
   *
   *   int findRowOfParameterValue(unsigned int value) const noexcept
   *   {
   *     const auto pos = mList.container().findPositionOfParameterValue(value);
   *     if( mList.positionIsInRange(pos) ){
   *       return mList.rowFromPosition(pos);
   *     }
   *     return -1;
   *   }
   *
   *  private:
   *
   *   int rowCountWithoutParentIndex() const override
   *   {
   *     return mList.rowCount();
   *   }
   *
   *   int columnCountWithoutParentIndex() const override
   *   {
   *     return 2;
   *   }
   *
   *   QVariant displayRoleData(const QModelIndex & index) const override
   *   {
   *     assert( indexIsValidAndInRange(index) );
   *
   *     switch( index.column() ){
   *       case 0:
   *         return mList.atRow( index.row() ).parameterValue(); // HEX formatting omitted here
   *       case 1:
   *         return mList.atRow( index.row() ).name();
   *     }
   *
   *     return QVariant();
   *   }
   *
   *   Mdt::ItemModel::StlContiguousContainerAdapter<InterfaceList, InterfaceListTableModelAdapterFunctionMap> mList;
   * };
   * \endcode
   *
   * Maybe the \a InterfaceList provides an index based find:
   * \code
   * class InterfaceList
   * {
   *   ...
   *
   *   using size_type = InterfaceList::size_type;
   *   using const_reference = const Interface &;
   *
   *   std::optional<size_type> findIndexOfParameterValue(unsigned int value) const noexcept;
   *
   *   ...
   * };
   * \endcode
   *
   * difference_type and const_iterator are no more required.
   * The function map also does not declare them anymore:
   * \code
   * struct InterfaceListTableModelAdapterFunctionMap
   * {
   *   using size_type = InterfaceList::size_type;
   *   using const_reference = const Interface &;
   *
   *   static
   *   size_type size(const InterfaceList & list) noexcept
   *   {
   *     return list.interfaceCount();
   *   }
   *
   *   static
   *   const_reference atIndex(const InterfaceList & list, size_type index) noexcept
   *   {
   *     return list.interfaceAt(index);
   *   }
   * };
   * \endcode
   * The cbegin() function is also no more required.
   *
   * Here is the updated table model's findRowOfParameterValue() method:
   * \code
   * class InterfaceListTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *   ...
   *
   *   int findRowOfParameterValue(unsigned int value) const noexcept
   *   {
   *     const auto index = mList.container().findIndexOfParameterValue(value);
   *     if( index.has_value() ){
   *       return mList.rowFromIndex(*index);
   *     }
   *     return -1;
   *   }
   *
   *   ...
   * };
   * \endcode
   *
   * Note that findRowOfParameterValue() returns -1 if given value was not found.
   * Because this method is our own helper (it's not part of the Qt model/view API, except that row must be int),
   * we could have chosen to adopt another strategy, like precondition that value exists,
   * or throwing an exception.
   *
   *
   * \sa SharedStlContiguousContainerAdapter
   * \sa AbstractTableModel
   * \sa StlContiguousContainerFunctionMap
   * \sa https://doc.qt.io/qt-6/qabstractitemmodel.html
   * \sa https://doc.qt.io/qt-6/qmodelindex.html
   * \sa https://en.cppreference.com/w/cpp/named_req/ContiguousContainer
   *
   *
   * \section Mdt_ItemModel_StlContiguousContainerAdapter_DifficultiesAdapterSolves Some difficulties this adapter helps to solve
   *
   * There are cases where we want to present a collection of elements,
   * based on a STL container, like std::vector,
   * by providing a Qt item model based access model.
   *
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
   *
   * \section Mdt_ItemModel_StlContiguousContainerAdapter_Rationale Rationale
   *
   * Here is some of the early sketch, that mainly focused on how to find
   * a row regarding a given condition:
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
     * otherwise void*.
     */
    using const_iterator = Mdt::TypeTraits::member_const_iterator_or_void_pointer<FunctionMap>;

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
     * \pre The current size of the container must be convertible to int.
     * The result must also be >= 0 and <= maxRowCount()
     */
    int rowCount() const
    {
      assert( Mdt::Numeric::int_canHoldValueOf_T( FunctionMap::size(mContainer) ) );

      const int count = Mdt::Numeric::int_from_T( FunctionMap::size(mContainer) );
      assert( count >= 0 );
      assert( count <= maxRowCount() );

      return count;
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

    /*! \brief Get the maximum allowed count of rows
     *
     * If the function map provides a function to get the container's maximum allowed size,
     * it will be used.
     *
     * This function must be of this form:
     * \code
     * static
     * size_type maxSize(const Container & container);
     * \endcode
     *
     * Returns the minimum value between:
     * - the largest possible value for int
     * - the largest possible value for size_type
     * - FunctionMap::maxSize() if defined
     */
    int maxRowCount() const
    {
      if constexpr( Mdt::TypeTraits::has_member_maxSize_container<FunctionMap, Container>() ){
        return minBetweenSizeTypeValueAndIntMax( FunctionMap::maxSize(mContainer) );
      }
      return minBetweenSizeTypeMaxAndIntMax<size_type>();
    }

    /*! \brief Check if given \a count rows can be added
     *
     * If rowCount() + \a count can't be represented by int,
     * returns false.
     *
     * If rowCount() + \a count exceeds maxRowCount(),
     * returns false.
     *
     * This also implies that rowCount() + \a count can be represented
     * by size_type .
     *
     * \pre \a count must be >= 1
     */
    bool canAddCountRows(int count) const
    {
      assert( count >= 1 );

      if( !Mdt::Numeric::canAdd(rowCount(), count) ){
        return false;
      }

      return (rowCount() + count) <= maxRowCount();
    }

    /*! \brief Inserts count rows into the container before the given row
     *
     * To use this method, the function map must have an insert function of this form:
     * \code
     * static
     * void insert(Container & container, const_iterator pos, size_type count, const_reference value);
     * \endcode
     *
     * To define pos, the function map must also have a const-qualified cbegin:
     * \code
     * static
     * const_iterator cbegin(const Container & container) noexcept;
     * \endcode
     *
     * \pre FunctionMap::difference_type must be defined
     * \pre FunctionMap::const_iterator must be defined
     * \pre \a row must be >= 0
     * \pre \a row must be <= rowCount()
     * \pre \a count must be >= 1
     * \pre it must be possible to add \a count rows
     */
    void insertRows(int row, int count, const_reference value)
    {
      static_assert( !std::is_void_v<difference_type>, "call StlContiguousContainerAdapter::insertRows() requires FunctionMap::difference_type to be defined" );
      static_assert( !Mdt::TypeTraits::is_void_or_void_pointer<const_iterator>(),
                     "call StlContiguousContainerAdapter::insertRows() requires FunctionMap::const_iterator to be defined" );
      assert( row >= 0 );
      assert( row <= rowCount() );
      assert( count >= 1 );
      assert( canAddCountRows(count) );

      insertToStlContainer<Container, FunctionMap>(mContainer, row, count, value);
    }

    /*! \brief Append an element
     *
     * To use this method, the function map must have a push_back function of this form:
     * \code
     * static
     * void push_back(Container & container, const_reference value);
     * \endcode
     *
     * \pre it must be possible to add 1 row
     */
    void appendRow(const_reference value)
    {
      assert( canAddCountRows(1) );

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
     * To define first and last, the function map must also have a const-qualified cbegin:
     * \code
     * static
     * const_iterator cbegin(const Container & container) noexcept;
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
      static_assert( !Mdt::TypeTraits::is_void_or_void_pointer<const_iterator>(),
                     "call StlContiguousContainerAdapter::removeRows() requires FunctionMap::const_iterator to be defined" );
      assert( row >= 0 );
      assert( count >= 1 );
      assert( Mdt::Numeric::canAdd(row, count) );
      assert( (row + count) > 0 );
      assert( (row + count) <= rowCount() );

      removeFromStlContainer<Container, FunctionMap>(mContainer, row, count);
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

    /*! \brief Get the row from given size_type index
     *
     * \pre \a index must be convertible to int
     * \pre \a index must be in range of the container ( 0 >= \a index < container's size )
     */
    int rowFromIndex(size_type index) const
    {
      assert( Mdt::Numeric::int_canHoldValueOf_T(index) );
      assert( index >= 0 );
      assert( index < FunctionMap::size(mContainer) );

      return Mdt::Numeric::int_from_T(index);
    }

    /*! \brief Check if given iterator \a pos is in range
     *
     * Returns true if \a pos is in range of the container,
     * and represents an index that is convertible to int.
     * Otherwise returns false.
     *
     * To use this method, the function map must have a cbegin() function of this form:
     * \code
     * static
     * const_iterator cbegin(const Container & container) noexcept;
     * \endcode
     *
     * \pre FunctionMap::difference_type must be defined
     * \pre FunctionMap::const_iterator must be defined
     */
    bool positionIsInRange(const_iterator pos) const
    {
      static_assert( !std::is_void_v<difference_type>, "call StlContiguousContainerAdapter::positionIsInRange() requires FunctionMap::difference_type to be defined" );
      static_assert( !Mdt::TypeTraits::is_void_or_void_pointer<const_iterator>(),
                     "call StlContiguousContainerAdapter::positionIsInRange() requires FunctionMap::const_iterator to be defined" );

      const difference_type dIndex = std::distance(FunctionMap::cbegin(mContainer), pos);
      if(dIndex < 0){
        return false;
      }
      if( !Mdt::Numeric::int_canHoldValueOf_T(dIndex) ){
        return false;
      }
      int row = Mdt::Numeric::int_from_T(dIndex);

      return row < rowCount();
    }

    /*! \brief Get the row from given iterator \a pos
     *
     * To use this method, the function map must have a cbegin() function of this form:
     * \code
     * static
     * const_iterator cbegin(const Container & container) noexcept;
     * \endcode
     *
     * \pre FunctionMap::difference_type must be defined
     * \pre FunctionMap::const_iterator must be defined
     * \pre \a pos must be in range
     * \sa positionIsInRange()
     */
    int rowFromPosition(const_iterator pos) const
    {
      static_assert( !std::is_void_v<difference_type>, "call StlContiguousContainerAdapter::rowFromPosition() requires FunctionMap::difference_type to be defined" );
      static_assert( !Mdt::TypeTraits::is_void_or_void_pointer<const_iterator>(),
                     "call StlContiguousContainerAdapter::rowFromPosition() requires FunctionMap::const_iterator to be defined" );
      assert( positionIsInRange(pos) );

      const difference_type dIndex = std::distance(FunctionMap::cbegin(mContainer), pos);
      assert( Mdt::Numeric::int_canHoldValueOf_T(dIndex) );

      int row = Mdt::Numeric::int_from_T(dIndex);
      assert( row < rowCount() );

      return row;
    }

    /*! \brief Access the container
     */
    const Container & container() const noexcept
    {
      return mContainer;
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
