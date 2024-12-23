// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "Helpers.h"
#include "Mdt/ItemModel/Helpers.h"
#include <QItemSelectionModel>
#include <QItemSelection>
#include <cassert>


namespace Mdt{ namespace ItemView{

bool removeSelectedRows(QAbstractItemView & view)
{
  assert( view.model() != nullptr );
  assert( view.selectionModel() != nullptr );
  assert( view.selectionModel()->model() == view.model() );

  return Mdt::ItemModel::removeSelectedRows( view.selectionModel() );
}

}} // namespace Mdt{ namespace ItemView{
