// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_TEST_LIB_ROW_SELECTION_HELPERS_H
#define MDT_ITEM_MODEL_TEST_LIB_ROW_SELECTION_HELPERS_H

#include "Mdt/ItemModel/RowSelection.h"
#include "mdt_itemmodel_testlib_export.h"
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <vector>

namespace Mdt{ namespace ItemModel{ namespace TestLib{

  /*! \brief Make a row selection from given index list
   *
   * \pre each index in \a indexList must be in range of \a model ( 0 <= index < model.rowCount() )
   *
   * \sa makeRowSelectionFromIndexList(const std::vector<int> &);
   */
  MDT_ITEMMODEL_TESTLIB_EXPORT
  RowSelection makeRowSelectionFromIndexList(const QAbstractItemModel & model, const std::vector<int> & indexList);

  /*! \brief Make a row selection from given index list
   *
   * \pre each index in \a indexList must be >= 0
   *
   * \sa makeRowSelectionFromIndexList(const QAbstractItemModel &, const std::vector<int> &);
   */
  MDT_ITEMMODEL_TESTLIB_EXPORT
  RowSelection makeRowSelectionFromIndexList(const std::vector<int> & indexList);

  /*! \brief Make an index list from given row selection
   */
  MDT_ITEMMODEL_TESTLIB_EXPORT
  std::vector<int> makeIndexListFromRowSelection(const RowSelection & selection) noexcept;

  /*! \brief Select items for given rows and column
   *
   * \pre \a selectionModel must refer to a model
   * \pre each couple of row in \a rows and \a column must be in the range
   * of the model referenced by \a selectionModel
   */
  MDT_ITEMMODEL_TESTLIB_EXPORT
  void selectRowsAndColumn(QItemSelectionModel & selectionModel,
                           const std::vector<int> & rows, int column,
                           QItemSelectionModel::SelectionFlags command = QItemSelectionModel::ClearAndSelect) noexcept;

}}} // namespace Mdt{ namespace ItemModel{ namespace TestLib{

#endif // #ifndef MDT_ITEM_MODEL_TEST_LIB_ROW_SELECTION_HELPERS_H
