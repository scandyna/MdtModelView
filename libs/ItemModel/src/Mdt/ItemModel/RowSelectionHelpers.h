// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_ROW_SELECTION_HELPERS_H
#define MDT_ITEM_MODEL_ROW_SELECTION_HELPERS_H

#include "Mdt/ItemModel/RowRange.h"
#include "mdt_itemmodel_export.h"
#include <QItemSelectionRange>

namespace Mdt{ namespace ItemModel{

  /*! \brief Get a row range from given item selection range
   *
   * \pre \a itemSelectionRange must be valid
   * \pre \a itemSelectionRange must not be empty
   */
  MDT_ITEMMODEL_EXPORT
  RowRange rowRangeFromItemSelectionRange(const QItemSelectionRange & itemSelectionRange) noexcept;

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_ROW_SELECTION_HELPERS_H
