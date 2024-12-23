// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_TEST_LIB_TABLE_MODEL_COMMON_BASE_H
#define MDT_ITEM_MODEL_TEST_LIB_TABLE_MODEL_COMMON_BASE_H

#include "Mdt/ItemModel/AbstractTableModel.h"
#include "Mdt/Numeric/BasicConversion.h"
#include "mdt_itemmodel_testlib_export.h"
#include <QVariant>
#include <vector>
#include <string>


namespace Mdt{ namespace ItemModel{ namespace TestLib{

  /*! \brief Common base to create basic table models for tests
   */
  class MDT_ITEMMODEL_TESTLIB_EXPORT TableModelCommonBase : public Mdt::ItemModel::AbstractTableModel
  {
    Q_OBJECT

   public:

    struct Record
    {
      int id = 0;
      std::string name;
    };

    using Table = std::vector<Record>;

    enum class Column
    {
      Id = 0,
      Name = 1
    };

    TableModelCommonBase(QObject *parent = nullptr)
    : AbstractTableModel(parent)
    {
    }

    void setTable(const Table & table)
    {
      mTable = table;
    }

   protected:

    /*
     * Following methods only manipulates the table.
     * They do NOT call methods like beginInsertRows().
     */

    void prependRecordToTable(const Record & record) noexcept;
    void insertRecordToTable(int row, int count, const Record & record) noexcept;
    void appendRecordToTable(const Record & record) noexcept;

    void removeFirstRowFromTable() noexcept;
    void removeRowsFromTable(int row, int count) noexcept;
    void removeLastRowFromTable() noexcept;

   private:

    int rowCountWithoutParentIndex() const noexcept override
    {
      return Mdt::Numeric::int_from_size_t( mTable.size() );
    }

    int columnCountWithoutParentIndex() const noexcept override
    {
      return 2;
    }

    QVariant displayRoleData(const QModelIndex & index) const noexcept override;

    Table mTable;
  };

}}} // namespace Mdt{ namespace ItemModel{ namespace TestLib{

#endif // #ifndef MDT_ITEM_MODEL_TEST_LIB_TABLE_MODEL_COMMON_BASE_H
