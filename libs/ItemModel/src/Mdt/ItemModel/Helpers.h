// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_HELPERS_H
#define MDT_ITEM_MODEL_HELPERS_H

#include "mdt_itemmodel_export.h"
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QVariant>

namespace Mdt{ namespace ItemModel{

  /*! \brief Check if given row and column are in the range of given model
   */
  MDT_ITEMMODEL_EXPORT
  bool modelRowAndColumnAreInRange(const QAbstractItemModel & model, int row, int column) noexcept;

  /*! \brief Get data from \a model at \a row and \a column for \a role
   *
   * \pre \a row must be in valid range ( 0 <= \a row < model->rowCount() )
   * \pre \a column must be in valid range ( 0 <= \a column < model->columnCount() )
   */
  MDT_ITEMMODEL_EXPORT
  QVariant getModelData(const QAbstractItemModel & model, int row, int column, Qt::ItemDataRole role = Qt::DisplayRole);

  /*! \brief Set \a value to \a model at \a row and \a column for \a role
   *
   * \pre \a row must be in valid range ( 0 <= \a row < model->rowCount() )
   * \pre \a column must be in valid range ( 0 <= \a column < model->columnCount() )
   */
  MDT_ITEMMODEL_EXPORT
  bool setModelData(QAbstractItemModel & model, int row, int column, const QVariant & value, Qt::ItemDataRole role = Qt::EditRole);

  /*! \brief Prepend a row to \a model
   */
  MDT_ITEMMODEL_EXPORT
  bool prependRowToModel(QAbstractItemModel & model);

  /*! \brief Append a row to \a model
   */
  MDT_ITEMMODEL_EXPORT
  bool appendRowToModel(QAbstractItemModel & model);

  /*! \brief Remove the first row from \a model
   *
   * If given model does not support removing the the first row,
   * or it is empty, false is returned,
   * otherwise true.
   */
  MDT_ITEMMODEL_EXPORT
  bool removeFirstRowFromModel(QAbstractItemModel & model);

  /*! \brief Remove the last row from \a model
   *
   * If given model does not support removing the the last row,
   * or it is empty, false is returned,
   * otherwise true.
   */
  MDT_ITEMMODEL_EXPORT
  bool removeLastRowFromModel(QAbstractItemModel & model);

  /*! \brief Remove selected rows from given model
   *
   * Will remove any row in which at least 1 item is selected.
   *
   * In this example, all rows will be removed:
   * |0|1|2|
   * |-|-|-|
   * |x| | |
   * | |x| |
   * | | |x|
   *
   * Here, only the first row (row 0) and the last (row 2) will be removed:
   * |0|1|2|
   * |-|-|-|
   * |x| | |
   * | | | |
   * | | |x|
   *
   * It does not matter how many items are selected in a row.
   * This example will result in the same result as before
   * (first and last row will be removed):
   * |0|1|2|
   * |-|-|-|
   * |x|x|x|
   * | | | |
   * | | |x|
   *
   * removeRows() will be called for each row in the selection,
   * in a descending way, on the model.
   * In this way, the row indexes should stay valid during the removal.
   * The above example will call:
   * \code
   * model->removeRows(2, 1);
   * model->removeRows(0, 1);
   * \endcode
   *
   * If contiguous rows are selected,
   * rows removals are done by chunks.
   *
   * |0|1|2|
   * |-|-|-|
   * |x| | |
   * |x| | |
   * | | | |
   * |x| | |
   * The above example will call:
   * \code
   * model->removeRows(3, 1);
   * model->removeRows(0, 2);
   * \endcode
   *
   * On the first removal that fails,
   * this function returns false.
   * If all removals succeeded, this function returns true.
   *
   * \pre \a selectionModel must be a valid pointer
   * \pre \a selectionModel must must refer to a model
   *
   * \sa Mdt::ItemView::removeSelectedRows()
   * \sa RowSelection
   *
   *
   * \todo make a variant that acts on some sort of checked rows
   * (i.e. a dedicated column, in which the user can intuitively select what to remove)
   * Could be named removeCheckedRows()
   */
  MDT_ITEMMODEL_EXPORT
  bool removeSelectedRows(QItemSelectionModel *selectionModel);

  /*! \brief Check if an item selection is a single row
   *
   * Returns true if given selection represents a single row,
   * otherwise false.
   *
   * The selection can also contain multiple columns that are all in the same row,
   * in which case it also represents a single row.
   */
  MDT_ITEMMODEL_EXPORT
  bool itemSelectionIsSingleRow(const QItemSelection & selection) noexcept;

  /*! \brief Update the row in given selection
   *
   * \pre \a row must be in the range of the model the selection refers to
   * \pre \a selection must be a single row selection
   * \sa itemSelectionIsSingleRow()
   */
  MDT_ITEMMODEL_EXPORT
  void updateRowInSingleRowItemSelection(int row, QItemSelection & selection) noexcept;

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_HELPERS_H
