// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_ABSTRACT_TABLE_MODEL_H
#define MDT_ITEM_MODEL_ABSTRACT_TABLE_MODEL_H

#include "mdt_itemmodel_export.h"
#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>
#include <QVector>

namespace Mdt{ namespace ItemModel{

  /*! \brief Provides a base to create table models
   *
   * The common way to create a custom table model is to subclass QAbstractTableModel .
   * While QAbstractTableModel provides a good API,
   * some boilerplate code has to be written many times.
   * The goal of AbstractTableModel is to reduce such code for the most common cases.
   *
   * Example for a simple read only table model:
   * \code
   * class ReadOnlyTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *   Q_OBJECT
   *
   *  public:
   *
   *   ReadOnlyTableModel(QObject *parent = nullptr)
   *    : AbstractTableModel(parent)
   *   {
   *   }
   *
   *  private:
   *
   *   int rowCountWithoutParentIndex() const noexcept override
   *   {
   *     return mTable.size();
   *   }
   *
   *   int columnCountWithoutParentIndex() const noexcept override
   *   {
   *     return 2;
   *   }
   *
   *   QVariant horizontalHeaderDisplayRoleData(int column) const noexcept override
   *   {
   *     assert( columnIndexIsInRange(column) );
   *
   *     switch(column){
   *       case 0:
   *         return "value";
   *       case 1:
   *         return "name";
   *     }
   *
   *     return QVariant();
   *   }
   *
   *   QVariant displayRoleData(const QModelIndex & index) const noexcept override
   *   {
   *     assert( indexIsValidAndInRange(index) );
   *
   *     return mTable[index.row()][index.column()];
   *   }
   *
   *   using Record = std::array<QVariant, 2>;
   *   using Table = std::vector<Record>;
   *
   *   Table mTable;
   * };
   * \endcode
   *
   * Sometimes we need other roles than Qt::DisplayRole:
   * \code
   * class CustomRolesTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *   Q_OBJECT
   *
   *  public:
   *
   *   CustomRolesTableModel(QObject *parent = nullptr)
   *    : AbstractTableModel(parent)
   *   {
   *   }
   *
   *   int sortRole() const noexcept
   *   {
   *     return Qt::UserRole;
   *   }
   *
   *  private:
   *
   *   // Methods identical to the ReadOnlyTableModel example omitted here
   *
   *   QVariant displayRoleData(const QModelIndex & index) const noexcept override
   *   {
   *     assert( indexIsValidAndInRange(index) );
   *
   *     switch( index.column() ){
   *       case 0:
   *         // Display id with a format: 000-00-00
   *         return formatId( mTable[index.row()].id() );
   *       case 1:
   *         return mTable[index.row()].name();
   *     }
   *
   *     return QVariant();
   *   }
   *
   *   QVariant otherRoleData(const QModelIndex & index, int role) const override
   *   {
   *     assert( indexIsValidAndInRange(index) );
   *
   *     if( role == Qt::FontRole ){
   *       return fontRoleData(index);
   *     }
   *     if( role == sortRole() ){
   *       return sortRoleData(index);
   *     }
   *
   *     return QVariant();
   *   }
   *
   *   QVariant fontRoleData(const QModelIndex & index) const noexcept
   *   {
   *     assert( indexIsValidAndInRange(index) );
   *
   *    // See Qt documentation
   *   }
   *
   *   QVariant sortRoleData(const QModelIndex & index) const noexcept
   *   {
   *     assert( indexIsValidAndInRange(index) );
   *
   *     if( index.column() == 0 ){
   *       // return the numeric value of id, so that the sorting is natural
   *       return mTable[index.row()].id().value();
   *     }
   *
   *     return displayRoleData(index);
   *   }
   * };
   * \endcode
   *
   * Example for an editable table model:
   * \code
   * class EditableTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  Q_OBJECT
   *
   *  public:
   *
   *   EditableTableModel(QObject *parent = nullptr)
   *    : AbstractTableModel(parent)
   *   {
   *   }
   *
   *   Qt::ItemFlags flags(const QModelIndex & index) const override
   *   {
   *     if( !indexIsValidAndInRange(index) ){
   *       return AbstractTableModel::flags(index);
   *     }
   *
   *     // Here we assume that all columns are editable
   *     return AbstractTableModel::flags(index) | Qt::ItemIsEditable;
   *   }
   *
   *  private:
   *
   *   // Methods identical to the ReadOnlyTableModel example omitted here
   *
   *   bool setEditRoleData(const QModelIndex & index, const QVariant & value) noexcept override
   *   {
   *     assert( indexIsValidAndInRange(index) );
   *
   *     switch( index.column() ){
   *       case 0:
   *         mTable[index.row()].setIdValue( value.toInt() );
   *         return true;
   *       case 1:
   *         mTable[index.row()].setName( value.toString() );
   *         return true;
   *     }
   *
   *     return false;
   *   }
   * };
   * \endcode
   *
   * Example of a model that does not support inserting rows at any valid place,
   * but can append a single row:
   * \code
   * class AppendRowTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  Q_OBJECT
   *
   *  public:
   *
   *   AppendRowTableModel(QObject *parent = nullptr)
   *    : AbstractTableModel(parent)
   *   {
   *   }
   *
   *  private:
   *
   *   // Methods identical to the ReadOnlyTableModel example omitted here
   *
   *   bool doSupportsAppendRow() const noexcept override
   *   {
   *     return true;
   *   }
   *
   *   void doAppendRow() noexcept override
   *   {
   *     mTable.push_back( Record() );
   *   }
   * };
   * \endcode
   *
   * Example of a model that provides a method
   * to add a specific record to the end:
   * \code
   * class AppendRecordTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  Q_OBJECT
   *
   *  public:
   *
   *   AppendRecordTableModel(QObject *parent = nullptr)
   *    : AbstractTableModel(parent)
   *   {
   *   }
   *
   *   void appendRecord(const Record & record)
   *   {
   *     beginAppendRow();
   *     mTable.push_back(record);
   *     endAppendRow();
   *   }
   *
   *  private:
   *
   *   // Methods identical to the ReadOnlyTableModel example omitted here
   * };
   * \endcode
   *
   * If the model supports inserting rows at any valid place,
   * it should only implement doInsertRows() for insertions:
   * \code
   * class InsertRowsTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  Q_OBJECT
   *
   *  public:
   *
   *   InsertRowsTableModel(QObject *parent = nullptr)
   *    : AbstractTableModel(parent)
   *   {
   *   }
   *
   *  private:
   *
   *   // Methods identical to the ReadOnlyTableModel example omitted here
   *
   *   bool doSupportsInsertRows() const noexcept override
   *   {
   *     return true;
   *   }
   *
   *   void doInsertRows(int row, int count) noexcept override
   *   {
   *     assert( rowAndCountIsValidForInsertRows(row, count) );
   *
   *     insertToStlContainer( mTable, row, count, Record() );
   *   }
   * };
   * \endcode
   *
   * Example of a model that does not support removing rows at any valid place,
   * but supports removing the last row:
   * \code
   * class RemoveLastRowTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  Q_OBJECT
   *
   *  public:
   *
   *   RemoveLastRowTableModel(QObject *parent = nullptr)
   *    : AbstractTableModel(parent)
   *   {
   *   }
   *
   *  private:
   *
   *   // Methods identical to the ReadOnlyTableModel example omitted here
   *
   *   bool doSupportsRemoveLastRow() const noexcept override
   *   {
   *     return true;
   *   }
   *
   *   void doRemoveLastRow() noexcept override
   *   {
   *     assert( !mTable.empty() );
   *
   *     mTable.pop_back();
   *   }
   * };
   * \endcode
   *
   * Example of a model that does support removing rows at any valid places.
   * It should only implement doRemoveRows() for removals:
   * \code
   * class RemoveRowsTableModel : public Mdt::ItemModel::AbstractTableModel
   * {
   *  Q_OBJECT
   *
   *  public:
   *
   *   RemoveRowsTableModel(QObject *parent = nullptr)
   *    : AbstractTableModel(parent)
   *   {
   *   }
   *
   *  private:
   *
   *   // Methods identical to the ReadOnlyTableModel example omitted here
   *
   *   bool doSupportsRemoveRows() const noexcept override
   *   {
   *     return true;
   *   }
   *
   *   void doRemoveRows(int row, int count) noexcept override
   *   {
   *     assert( rowAndCountIsValidForRemoveRows(row, count) );
   *
   *     removeFromStlContainer(mTable, row, count);
   *   }
   * };
   * \endcode
   *
   * \todo We should remove noexcept in the contract.
   * Think about models that maybe fetches data from file, DB, etc..
   * Thera are also incoherences between displayRoleData() , editRoleData() , setDisplayRoleData() , setEditRoleData() ...
   */
  class MDT_ITEMMODEL_EXPORT AbstractTableModel : public QAbstractTableModel
  {
   Q_OBJECT

   public:

    /*! \brief Construct an abstract table model
     */
    explicit AbstractTableModel(QObject *parent = nullptr) noexcept;

    /*! \brief Get count of rows
     *
     * Will call rowCountWithoutParent() if parent is not valid,
     * otherwise returns 0.
     */
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;

    /*! \brief Get count of columns
     *
     * Will call columnCountWithoutParent() if parent is not valid,
     * otherwise returns 0.
     */
    int columnCount(const QModelIndex & parent = QModelIndex()) const override;

    /*! \brief Check if given row index is in range
     *
     * Returns true if: 0 <= \a row < rowCount()
     *
     * \sa indexIsValidAndInRange()
     */
    bool rowIndexIsInRange(int row) const noexcept
    {
      if(row < 0){
        return false;
      }

      return row < rowCountWithoutParentIndex();
    }

    /*! \brief Check if given column index is in range
     *
     * Returns true if: 0 <= \a column < columnCount()
     *
     * \sa indexIsValidAndInRange()
     */
    bool columnIndexIsInRange(int column) const noexcept
    {
      if(column < 0){
        return false;
      }

      return column < columnCountWithoutParentIndex();
    }

    /*! \brief Get the header data at \a section for \a orientation and \a role
     *
     * If \a orientation is Qt::Horizontal and \a role is Qt::DisplayRole,
     * horizontalHeaderDisplayRoleData() is called.
     *
     * If \a orientation is Qt::Vertical and \a role is Qt::DisplayRole,
     * verticalHeaderDisplayRoleData() is called.
     *
     * For more complex models that supports other roles than Qt::DisplayRole,
     * this method should be re-implemented.
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /*! \brief Check if given index is valid and in the range of this table model
     *
     * \sa rowIndexIsInRange()
     * \sa columnIndexIsInRange()
     */
    bool indexIsValidAndInRange(const QModelIndex & index) const noexcept;

    /*! \brief Get the data at \a index for \a role
     *
     * If \a index is valid and in range, the following method will be called:
     *  - displayRoleData() if \a role is Qt::DisplayRole
     *  - editRoleData() if \a role is Qt::EditRole
     *  - otherRoleData() if \a role is none of the above ones
     *
     * \note For other stuff as data access, for example formating,
     *   using a proxy model should be a good solution.
     */
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    /*! \brief Set data for \a role at \a index
     *
     * If \a index is valid and in range, the following method will be called:
     *  - setEditRoleData() if \a role is Qt::EditRole
     *  - setDisplayRoleData() if \a role is Qt::DisplayRole
     *  - setOtherRoleData() if \a role is none of the above ones
     *
     * If above methods return true, the dataChanged() signal will be emitted for \a index .
     * All roles will be considered modified, which is a good default
     * (maybe some formating depends on the data at given index, which will be updated also).
     *
     * \note If data should be updated, but not by the user,
     * setData() should be called with Qt::DisplayRole .
     * If the model has to handle setting display role data differently than edit role data,
     * it should reimplement setDisplayRoleData(), otherwise it is not required.
     *
     * \note If many data for a row has to be updated (not from the user),
     * a specific method should be implemented.
     * This one should emit dataChanged() only once for the complete set of updated items in the row.
     * This prevents strange behaviour, as example, when using dynamic sorting.
     * Emitting dataChanged() for each item in the row will apply sorting every time,
     * which can end up to missmatched data:
     * \code
     * int row = ....
     * setModelData(view->model(), row, idColumn, id, Qt::DisplayRole);
     * // emits dataChanged(), sort happend, now row points probably to another row in the model
     * setModelData(view->model(), row, descriptionColumn, description, Qt::DisplayRole);
     * // description is probably not in the row attached to id
     * \endcode
     *
     * \sa setEditRoleData()
     * \sa setDisplayRoleData()
     * \sa emitRowDataChanged()
     */
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;

    /*! \brief Check if this model supports prepending a row
     *
     * \sa supportsInsertRows()
     * \sa insertRows()
     * \sa doSupportsPrependRow()
     */
    bool supportsPrependRow() const noexcept
    {
      return doSupportsPrependRow();
    }

    /*! \brief Check if this model supports appending a row
     *
     * \sa supportsInsertRows()
     * \sa insertRows()
     * \sa doSupportsAppendRow()
     */
    bool supportsAppendRow() const noexcept
    {
      return doSupportsAppendRow();
    }

    /*! \brief Check if this model supports inserting rows
     *
     * Retruns true if the model supports inserting 1 or more rows,
     * at the beginning, the middle or the end.
     *
     * \sa supportsPrependRow()
     * \sa supportsAppendRow()
     * \sa insertRows()
     * \sa doSupportsInsertRows()
     */
    bool supportsInsertRows() const noexcept
    {
      return doSupportsInsertRows();
    }

    /*! \brief Check if given set of row and count is valid to insert rows
     *
     * A row < 0 is not valid.
     *
     * A row > rowCount() is not valid.
     *
     * A count < 1 is not valid.
     *
     * \sa insertRows()
     */
    bool rowAndCountIsValidForInsertRows(int row, int count) const noexcept;

    /*! \brief Check if given row and count represents prepending a row
     *
     * \pre \a row and \a count must be valid for insert rows
     * \sa rowAndCountIsValidForInsertRows()
     */
    bool rowAndCountRepresentsPrependRow(int row, int count) const noexcept;

    /*! \brief Check if given row and count represents appending a row
     *
     * \pre \a row and \a count must be valid for insert rows
     * \sa rowAndCountIsValidForInsertRows()
     */
    bool rowAndCountRepresentsAppendRow(int row, int count) const noexcept;

    /*! \brief Insert \a count rows before \a row into the model
     *
     * On models that support this, inserts count rows into the model before the given row.
     *
     * If \a parent is valid, this method does nothing and returns false.
     *
     * If \a row and \a count are not valid values for an insertion,
     * this method does nothing and returns false.
     *
     * If the implementation supports inserting rows,
     * doInsertRows() will be called.
     *
     * If \a row and \a count represents an insertion of 1 row at the beginning of this model,
     * and the implementation supports prepending a row, doPrependRow() will be called.
     *
     * If \a row and \a count represents an insertion of 1 row at the end of this model,
     * and the implementation supports appending a row, doAppendRow() will be called.
     *
     * If any row can be inserted, beginInsertRows() and endInsertRows() will be called from this method.
     *
     * \sa rowAndCountIsValidForInsertRows()
     * \sa rowAndCountRepresentsPrependRow()
     * \sa rowAndCountRepresentsAppendRow()
     * \sa supportsPrependRow()
     * \sa supportsAppendRow()
     * \sa supportsInsertRows()
     */
    bool insertRows( int row, int count, const QModelIndex & parent = QModelIndex() ) override;

    /*! \brief Check if this model supports removing the first row
     *
     * \sa supportsRemoveRows()
     * \sa removeRows()
     * \sa doSupportsRemoveFirstRow()
     */
    bool supportsRemoveFirstRow() const noexcept
    {
      return doSupportsRemoveFirstRow();
    }

    /*! \brief Check if this model supports removing the last row
     *
     * \sa supportsRemoveRows()
     * \sa removeRows()
     * \sa doSupportsRemoveLastRow()
     */
    bool supportsRemoveLastRow() const noexcept
    {
      return doSupportsRemoveLastRow();
    }

    /*! \brief Check if this model supports removing rows
     *
     * Retruns true if the model supports removing 1 or more rows,
     * at the beginning, the middle or the end.
     *
     * \sa supportsRemoveFirstRow()
     * \sa supportsRemoveLastRow()
     * \sa removeRows()
     * \sa doSupportsRemoveRows()
     */
    bool supportsRemoveRows() const noexcept
    {
      return doSupportsRemoveRows();
    }

    /*! \brief Check if given set of row and count is valid to remove rows
     *
     * A row < 0 is not valid.
     *
     * A count < 1 is not valid.
     *
     * ( \a row + \a count ) > rowCount() is not valid.
     */
    bool rowAndCountIsValidForRemoveRows(int row, int count) const noexcept;

    /*! \brief Check if given row and count represents removing the first row
     *
     * \pre \a row and \a count must be valid for removing rows
     * \sa rowAndCountIsValidForRemoveRows()
     */
    bool rowAndCountRepresentsRemoveFirstRow(int row, int count) const noexcept;

    /*! \brief Check if given row and count represents removing the last row
     *
     * \pre \a row and \a count must be valid for removing rows
     * \sa rowAndCountIsValidForRemoveRows()
     */
    bool rowAndCountRepresentsRemoveLastRow(int row, int count) const noexcept;

    /*! \brief Remove \a count rows starting from \a row
     *
     * On models that support this, removes \a count rows starting with the given row.
     *
     * If \a parent is valid, this method does nothing and returns false.
     *
     * If \a row and \a count are not valid values for a removal,
     * this method does nothing and returns false.
     *
     * If the implementation supports removing rows at any valid place,
     * doRemoveRows() will be called.
     *
     * If \a row and \a count represents an removal of 1 row at the beginning of this model,
     * and the implementation supports removing a row at the beginning, doRemoveFirstRow() will be called.
     *
     * If \a row and \a count represents an removal of 1 row at the end of this model,
     * and the implementation supports removing a row at the end, doRemoveLastRow() will be called.
     *
     * \sa rowAndCountIsValidForRemoveRows()
     * \sa rowAndCountRepresentsRemoveFirstRow()
     * \sa rowAndCountRepresentsRemoveLastRow()
     * \sa supportsRemoveRows()
     * \sa supportsRemoveFirstRow()
     * \sa supportsRemoveLastRow()
     */
    bool removeRows( int row, int count, const QModelIndex & parent = QModelIndex() ) override;

   protected:

    /*! \brief Get count of rows
     *
     * This method is called by rowCount(const QModelIndex & parent)
     * only if given parent is not valid.
     */
    virtual
    int rowCountWithoutParentIndex() const noexcept = 0;

    /*! \brief Get count of columns
     *
     * This method is called by columnCount(const QModelIndex & parent)
     * only if given parent is not valid.
     */
    virtual
    int columnCountWithoutParentIndex() const noexcept = 0;

    /*! \brief Get the display role data for the horizontal header
     *
     * This method is called by headerData() if all preconditions are satisfied.
     *
     * This default implementation returns the implementation from QAbstractTableModel .
     *
     * \pre \a column must be in valid range
     * \sa columnIndexIsInRange()
     */
    virtual
    QVariant horizontalHeaderDisplayRoleData(int column) const noexcept;

    /*! \brief Get the display role data for the vertical header
     *
     * This method is called by headerData() if all preconditions are satisfied.
     *
     * This default implementation returns the implementation from QAbstractTableModel .
     *
     * \pre \a row must be in valid range
     * \sa rowIndexIsInRange()
     */
    virtual
    QVariant verticalHeaderDisplayRoleData(int row) const noexcept;

    /*! \brief Get display role data
     *
     * This method is called by data() if all preconditions are satisfied.
     *
     * \pre \a index must be valid and in range
     * \sa indexIsValidAndInRange()
     */
    virtual
    QVariant displayRoleData(const QModelIndex & index) const noexcept = 0;

    /*! \brief Get edit role data
     *
     * If the table model has to display specific data while editing an item,
     * this method can be re-implemented.
     *
     * This implementation returns displayRoleData() .
     *
     * This method is called by data() if all preconditions are satisfied.
     *
     * \pre \a index must be valid and in range
     * \sa indexIsValidAndInRange()
     */
    virtual
    QVariant editRoleData(const QModelIndex & index) const;

    /*! \brief Get other role data
     *
     * If the table model has to return data for roles that are not proposed
     * by this abstract table model (such as displayRoleData(), editRoleData() ),
     * this method can be re-implemented.
     *
     * This implementation returns a null QVariant.
     *
     * This method is called by data() if all preconditions are satisfied.
     *
     * \pre \a index must be valid and in range
     * \sa indexIsValidAndInRange()
     */
    virtual
    QVariant otherRoleData(const QModelIndex & index, int role) const;

    /*! \brief Set edit role data
     *
     * This method is called by setData() if all preconditions are satisfied.
     *
     * This implementation does nothing and returns false.
     *
     * \pre \a index must be valid and in range
     * \sa indexIsValidAndInRange()
     */
    virtual
    bool setEditRoleData(const QModelIndex & index, const QVariant & value) noexcept;

    /*! \brief Set display role data
     *
     * If the model has to handle display role data differently than edit role data,
     * this method can be re-implemented.
     *
     * This method is called by setData() if all preconditions are satisfied.
     *
     * This implementation calls setEditRoleData().
     *
     * \pre \a index must be valid and in range
     * \sa indexIsValidAndInRange()
     */
    virtual
    bool setDisplayRoleData(const QModelIndex & index, const QVariant & value) noexcept;

    /*! \brief Set data for given role
     *
     * If the table model can be edited with data for roles that are not proposed
     * by this abstract table model (such as setDisplayRoleData(), setEditRoleData() ),
     * this method can be re-implemented.
     *
     * This method is called by setData() if all preconditions are satisfied.
     *
     * This implementation does nothing and returns false.
     *
     * \pre \a index must be valid and in range
     * \sa indexIsValidAndInRange()
     */
    virtual
    bool setOtherRoleData(const QModelIndex & index, const QVariant & value, int role);

    /*! \brief Emit dataChanged() for a row
     *
     * This is a simple helper function that emit dataChanged() for given row.
     *
     * \pre this model must contain at least 1 column
     * \pre \a row must be in range
     * \sa rowIndexIsInRange()
     */
    void emitRowDataChanged( int row, const QVector<int> & roles = QVector<int>() ) noexcept;

    /*! \brief Begins a row append operation
     *
     * This is a helper to beginInsertRows().
     * It avoids thinking about calculating first and last correctly.
     *
     * \note when implementing doAppendRow(),
     * this method has NOT to be called.
     *
     * \sa endAppendRow()
     */
    void beginAppendRow();

    /*! \brief Ends a row append operation
     *
     * This helper calls endInsertRows().
     * It is provided to be coherent with beginAppendRow().
     *
     * \sa beginAppendRow()
     */
    void endAppendRow();

    /*! \brief Check if this model supports prepending a row
     *
     * If the implementation does not support inserting rows at any valid place,
     * but supports prepending a single row,
     * this method should be reimplemented and return true.
     *
     * In that case, doPrependRow() should be implemented.
     *
     * This default implementation returns false.
     *
     * \sa doPrependRow()
     * \sa supportsPrependRow()
     * \sa doSupportsInsertRows()
     */
    virtual
    bool doSupportsPrependRow() const noexcept
    {
      return false;
    }

    /*! \brief Check if this model supports appending a row
     *
     * If the implementation does not support inserting rows at any valid place,
     * but supports appending a single row,
     * this method should be reimplemented and return true.
     *
     * In that case, doAppendRow() should be implemented.
     *
     * This default implementation returns false.
     *
     * \sa doAppendRow()
     * \sa supportsAppendRow()
     * \sa doSupportsInsertRows()
     */
    virtual
    bool doSupportsAppendRow() const noexcept
    {
      return false;
    }

    /*! \brief Check if this model supports inserting rows
     *
     * If the implementation supports inserting rows at any valid place
     * (at the beginning, in the middle, at the end),
     * this method should be reimplemented and return true.
     *
     * In that case, doInsertRows() should also be implemented.
     *
     * This default implementation returns false.
     *
     * \sa doInsertRows()
     * \sa supportsInsertRows()
     */
    virtual
    bool doSupportsInsertRows() const noexcept
    {
      return false;
    }

    /*! \brief Prepend a row into the model
     *
     * This default implementation does nothing.
     *
     * \sa doSupportsPrependRow()
     */
    virtual
    void doPrependRow() noexcept
    {
    }

    /*! \brief Append a row into the model
     *
     * This default implementation does nothing.
     *
     * \note when implementing this method,
     * beginAppendRow() / endAppendRow() have NOT to be called.
     *
     * \sa doSupportsAppendRow()
     */
    virtual
    void doAppendRow() noexcept
    {
    }

    /*! \brief Insert \a count rows before \a row into the model
     *
     * This default implementation does nothing.
     *
     * \sa doSupportsInsertRows()
     */
    virtual
    void doInsertRows(int row, int count) noexcept;

    /*! \brief Check if this model supports removing the first row
     *
     * If the implementation does not support removing rows at any valid place,
     * but supports removing the first row,
     * this method should be reimplemented and return true.
     *
     * In that case, doRemoveFirstRow() should be implemented.
     *
     * This default implementation returns false.
     *
     * \sa doRemoveFirstRow()
     * \sa supportsRemoveFirstRow()
     * \sa doSupportsRemoveRows()
     */
    virtual
    bool doSupportsRemoveFirstRow() const noexcept
    {
      return false;
    }

    /*! \brief Check if this model supports removing the last row
     *
     * If the implementation does not support removing rows at any valid place,
     * but supports removing the last row,
     * this method should be reimplemented and return true.
     *
     * In that case, doRemoveLastRow() should be implemented.
     *
     * This default implementation returns false.
     *
     * \sa doRemoveLastRow()
     * \sa supportsRemoveLastRow()
     * \sa doSupportsRemoveRows()
     */
    virtual
    bool doSupportsRemoveLastRow() const noexcept
    {
      return false;
    }

    /*! \brief Check if this model supports removing rows
     *
     * If the implementation supports removing rows at any valid place
     * (at the beginning, in the middle, at the end),
     * this method should be reimplemented and return true.
     *
     * In that case, doRemoveRows() should also be implemented.
     *
     * This default implementation returns false.
     *
     * \sa doRemoveRows()
     * \sa supportsRemoveRows()
     */
    virtual
    bool doSupportsRemoveRows() const noexcept
    {
      return false;
    }

    /*! \brief Remove the first row from the model
     *
     * This default implementation does nothing.
     *
     * \sa doSupportsRemoveFirstRow()
     */
    virtual
    void doRemoveFirstRow() noexcept
    {
    }

    /*! \brief Remove the last row from the model
     *
     * This default implementation does nothing.
     *
     * \sa doSupportsRemoveLastRow()
     */
    virtual
    void doRemoveLastRow() noexcept
    {
    }

    /*! \brief Remove \a count rows starting from \a row
     *
     * This default implementation does nothing.
     *
     * \sa doSupportsRemoveRows()
     */
    virtual
    void doRemoveRows(int row, int count) noexcept;
  };

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_ABSTRACT_TABLE_MODEL_H
