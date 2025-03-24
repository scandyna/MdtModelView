// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2025-2025 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_SHARED_STL_CONTIGUOUS_CONTAINER_ADAPTER_H
#define MDT_ITEM_MODEL_SHARED_STL_CONTIGUOUS_CONTAINER_ADAPTER_H

#include <Mdt/Numeric/Limits.h>
#include <Mdt/Numeric/BasicConversion.h>
#include <memory>
#include <cassert>

namespace Mdt{ namespace ItemModel{

  /*! \brief Adapter to use shared STL contiguous containers with Qt item models
   *
   * This adapter can be used the same way as StlContiguousContainerAdapter.
   * The difference is that the container not owned by this adapter,
   * but managed with a shared pointer.
   *
   * Example of some container:
   * \code
   * class MyList
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
   * struct MyListTableModelAdapterFunctionMap
   * {
   *   using size_type = MyList::size_type;
   *   using const_reference = const Item &;
   *
   *   static
   *   size_type size(const MyList & list) noexcept
   *   {
   *     return list.getSizeCustom();
   *   }
   *
   *   static
   *   const_reference atIndex(const MyList & list, size_type index) noexcept
   *   {
   *     return list.itemAt(index);
   *   }
   * };
   * \endcode
   *
   * Now we can implement the table model:
   * \code
   * class MyListTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  public:
   *
   *   MyListTableModel(QObject *parent = nullptr)
   *    : AbstractTableModel(parent)
   *   {
   *   }
   *
   *   void setList(std::shared_ptr<MyList> list)
   *   {
   *     beginResetModel();
   *     mList.setContainer(list);
   *     endResetModel();
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
   *   Mdt::ItemModel::SharedStlContiguousContainerAdapter<MyList, MyListTableModelAdapterFunctionMap> mList;
   * };
   * \endcode
   *
   * Notice that we don't have to care about int / size_type conversions.
   * Also, as long as no container was set, rowCount() returns 0. 
   *
   * \todo Discuss default constructed:
   * - Should it exist in adapter ? Yes
   * - Should it be imposed ?
   * - For the shared version, should it instanciate an empty container, or be a nullptr ??
   *   For the nullptr version, noexcept + not imposes default constructible + no CPU waste
   *   But, should be able to construct the container on insert (?)
   *
   * \todo Put usage example
   *
   * \code
   * class MyTableModel : QAbstractTableModel
   * {
   *  public:
   *
   *   void setContainer(std::shared_ptr<MyContainer> container)
   *   {
   *     mContainer = container;
   *   }
   *
   *  private:
   *
   *   SharedStlContiguousContainerAdapter<MyContainer> mContainer;
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
   * \sa StlContiguousContainerAdapter
   * \sa AbstractTableModel
   */
  template<typename Container, typename FunctionMap>
  class SharedStlContiguousContainerAdapter
  {
   public:

    /*! \brief Construct an empty container
     *
     * \todo define what it does
     *
     * \todo Think about size() and at()
     * size(): check null
     * at(): precondition in range, impies not null
     */
    explicit
    SharedStlContiguousContainerAdapter() noexcept = default;

    /*! \brief Get the count of elements
     */
    int size() const noexcept
    {
      if(mContainer){
      }
      return 0;
    }


   private:

    std::shared_ptr<Container> mContainer;
  };

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_SHARED_STL_CONTIGUOUS_CONTAINER_ADAPTER_H
