// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_VIEW_HELPERS_H
#define MDT_ITEM_VIEW_HELPERS_H

#include "mdt_itemview_qtwidgets_export.h"
#include <QAbstractItemView>

namespace Mdt{ namespace ItemView{

  /*! \brief Remove the rows selected in given view
   *
   * \sa Mdt::ItemModel::removeSelectedRows()
   *
   * \pre \a view must refrence a model
   * \pre \a view must refrence a selection model
   * \pre the selection model must reference view's model
   */
  MDT_ITEMVIEW_QTWIDGETS_EXPORT
  bool removeSelectedRows(QAbstractItemView & view);

  /*! \brief
   *
   * \todo function to resize row/columns to contents
   * for the visible parts (+ some offset)
   */

}} // namespace Mdt{ namespace ItemView{

#endif // #ifndef MDT_ITEM_VIEW_HELPERS_H
