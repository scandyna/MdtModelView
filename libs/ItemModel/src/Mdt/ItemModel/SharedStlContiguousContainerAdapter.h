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

#include "Mdt/TypeTraits/StlContainerHelpers.h"
#include "Mdt/ItemModel/StlHelpers.h"
#include "Mdt/ItemModel/NumericLimits.h"
#include <Mdt/Numeric/Limits.h>
#include <Mdt/Numeric/BasicConversion.h>
#include <type_traits>
#include <memory>
#include <cassert>

namespace Mdt{ namespace ItemModel{

  /*! \brief Adapter to use shared STL contiguous containers with Qt item models
   *
   * This adapter can be used the same way as StlContiguousContainerAdapter.
   * The difference is that the container is not owned by this adapter,
   * but referenced with a shared pointer.
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
   * \sa StlContiguousContainerAdapter
   * \sa AbstractTableModel
   */
  template<typename Container, typename FunctionMap>
  class SharedStlContiguousContainerAdapter
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

    /*! \brief Construct an empty adapter
     *
     * An empty adapter does not reference any container,
     * it holds a null pointer.
     *
     * \sa setContainer()
     */
    explicit
    SharedStlContiguousContainerAdapter() noexcept = default;

    /*! \brief Set the container
     *
     * \pre \a container must be a valid pointer
     */
    void setContainer(std::shared_ptr<Container> container)
    {
      assert(container != nullptr);

      mContainer = container;
    }

    /*! \brief Get the maximum allowed count of rows
     *
     * If this adaper does not reference a container,
     * this method returns 0.
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
      if(!mContainer){
        return 0;
      }
      if constexpr( Mdt::TypeTraits::has_member_maxSize_container<FunctionMap, Container>() ){
        assert(mContainer != nullptr);
        return minBetweenSizeTypeValueAndIntMax( FunctionMap::maxSize(*mContainer) );
      }
      return minBetweenSizeTypeMaxAndIntMax<size_type>();
    }

    /*! \brief Get the count of rows for the model
     *
     * Returns the size of the container if this adapter references one,
     * otherwise 0.
     *
     * \pre The current size of the container must be convertible to int.
     * The result must also be >= 0 and <= maxRowCount()
     *
     * \sa setContainer()
     */
    int rowCount() const
    {
      if(mContainer){
        assert( Mdt::Numeric::int_canHoldValueOf_T( FunctionMap::size(*mContainer) ) );

        const int count = Mdt::Numeric::int_from_T( FunctionMap::size(*mContainer) );
        assert( count >= 0 );
        assert( count <= maxRowCount() );

        return count;
      }
      return 0;
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

    /*! \brief Get the element at given row
     *
     * \pre \a row must be in range of the container ( 0 >= \a row < rowCount() )
     * This also implies that a container has been set.
     * \sa setContainer()
     * \sa rowCount()
     */
    const_reference atRow(int row) const
    {
      assert( row >= 0 );
      assert( row < rowCount() );
      assert(mContainer != nullptr);

      return FunctionMap::atIndex( *mContainer, indexFromRow(row) );
    }

    /*! \brief Access the element at given row for mutation
     *
     * \pre the reference type must be valid.
     * \pre \a row must be in range ( 0 >= \a row < rowCount() )
     * This also implies that a container has been set.
     * \sa setContainer()
     * \sa rowCount()
     */
    reference atRowMutable(int row) noexcept
    {
      static_assert( !std::is_void_v<reference>, "call SharedStlContiguousContainerAdapter::atRowMutable() requires FunctionMap::reference to be defined" );
      assert( row >= 0 );
      assert( row < rowCount() );
      assert(mContainer != nullptr);

      return FunctionMap::atIndexMutable( *mContainer, indexFromRow(row) );
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
     * This also implies that a container has been set.
     * \sa setContainer()
     * \sa canAddCountRows()
     */
    void insertRows(int row, int count, const_reference value)
    {
      static_assert( !std::is_void_v<difference_type>, "call SharedStlContiguousContainerAdapter::insertRows() requires FunctionMap::difference_type to be defined" );
      static_assert( !Mdt::TypeTraits::is_void_or_void_pointer<const_iterator>(),
                     "call SharedStlContiguousContainerAdapter::insertRows() requires FunctionMap::const_iterator to be defined" );
      assert( row >= 0 );
      assert( row <= rowCount() );
      assert( count >= 1 );
      assert( canAddCountRows(count) );
      assert( mContainer != nullptr );

      insertToStlContainer<Container, FunctionMap>(*mContainer, row, count, value);
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
     * This also implies that a container has been set.
     * \sa setContainer()
     * \sa canAddCountRows()
     */
    void appendRow(const_reference value)
    {
      assert( canAddCountRows(1) );
      assert( mContainer != nullptr );

      FunctionMap::push_back(*mContainer, value);
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
     * This also implies that a container has been set.
     * \sa setContainer()
     * \sa rowCount()
     */
    void removeRows(int row, int count)
    {
      static_assert( !std::is_void_v<difference_type>, "call SharedStlContiguousContainerAdapter::removeRows() requires FunctionMap::difference_type to be defined" );
      static_assert( !Mdt::TypeTraits::is_void_or_void_pointer<const_iterator>(),
                     "call SharedStlContiguousContainerAdapter::removeRows() requires FunctionMap::const_iterator to be defined" );
      assert( row >= 0 );
      assert( count >= 1 );
      assert( Mdt::Numeric::canAdd(row, count) );
      assert( (row + count) > 0 );
      assert( (row + count) <= rowCount() );

      removeFromStlContainer<Container, FunctionMap>(*mContainer, row, count);
    }

    /*! \brief Get the size_type index from given row
     *
     * \pre \a row must be in range of the container ( 0 >= \a row < rowCount() )
     * This also implies that a container has been set.
     * \sa setContainer()
     * \sa rowCount()
     */
    size_type indexFromRow(int row) const
    {
      assert( row >= 0 );
      assert( row < rowCount() );
      assert( mContainer != nullptr );

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
     * \pre This adapter must reference a container
     * \sa setContainer()
     */
    int rowFromIndex(size_type index) const
    {
      assert( Mdt::Numeric::int_canHoldValueOf_T(index) );
      assert( index >= 0 );
      assert( mContainer != nullptr );
      assert( index < FunctionMap::size(*mContainer) );

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
     * \pre This adapter must reference a container
     * \sa setContainer()
     */
    bool positionIsInRange(const_iterator pos) const
    {
      static_assert( !std::is_void_v<difference_type>, "call SharedStlContiguousContainerAdapter::positionIsInRange() requires FunctionMap::difference_type to be defined" );
      static_assert( !Mdt::TypeTraits::is_void_or_void_pointer<const_iterator>(),
                     "call SharedStlContiguousContainerAdapter::positionIsInRange() requires FunctionMap::const_iterator to be defined" );
      assert(mContainer != nullptr);

      return positionIsInRangeOfStlContainer<Container, FunctionMap>(*mContainer, pos);
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
     * \pre This adapter must reference a container
     * \sa setContainer()
     */
    int rowFromPosition(const_iterator pos) const
    {
      static_assert( !std::is_void_v<difference_type>, "call SharedStlContiguousContainerAdapter::rowFromPosition() requires FunctionMap::difference_type to be defined" );
      static_assert( !Mdt::TypeTraits::is_void_or_void_pointer<const_iterator>(),
                     "call SharedStlContiguousContainerAdapter::rowFromPosition() requires FunctionMap::const_iterator to be defined" );
      assert(mContainer != nullptr);
      assert( positionIsInRange(pos) );

      return indexFromPositionInStlContainer<Container, FunctionMap>(*mContainer, pos);
    }

    /*! \brief Access the container
     *
     * \pre This adapter must reference a container
     * \sa setContainer()
     * \sa containerMutablePtr()
     */
    const Container & container() const noexcept
    {
      assert(mContainer != nullptr);

      return *mContainer;
    }

    /*! \brief Access the container for mutation
     *
     * \pre This adapter must reference a container
     * \sa setContainer()
     * \sa containerMutablePtr()
     */
    Container & containerMutable() noexcept
    {
      assert(mContainer != nullptr);

      return *mContainer;
    }

    /*! \brief Access the container for mutation
     *
     * Returns the pointer to the container.
     *
     * Returns a nullptr if this adapter references no container.
     *
     * \sa setContainer()
     */
    std::shared_ptr<Container> containerMutablePtr() noexcept
    {
      return mContainer;
    }

   private:

    std::shared_ptr<Container> mContainer;
  };

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_SHARED_STL_CONTIGUOUS_CONTAINER_ADAPTER_H
