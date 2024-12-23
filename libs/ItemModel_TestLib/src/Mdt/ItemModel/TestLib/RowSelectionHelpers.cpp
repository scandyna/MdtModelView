// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "RowSelectionHelpers.h"
#include "Mdt/ItemModel/RowListView.h"
#include <QStringList>
#include <QStringListModel>
#include <QItemSelection>
#include <QModelIndex>
#include <algorithm>
#include <iterator>
#include <cassert>

namespace Mdt{ namespace ItemModel{ namespace TestLib{

RowSelection makeRowSelectionFromIndexList(const QAbstractItemModel & model, const std::vector<int> & indexList)
{
  QItemSelection itemSelection;

  for(int i : indexList){
    assert( i >= 0 );
    assert( i < model.rowCount() );
    QModelIndex index = model.index(i, 0);
    assert( index.isValid() );
    itemSelection.append( QItemSelectionRange(index) );
  }

  return RowSelection::fromItemSelection(itemSelection);
}

RowSelection makeRowSelectionFromIndexList(const std::vector<int> & indexList)
{
  const auto it = std::max_element( indexList.cbegin(), indexList.cend() );
  if( it == indexList.cend() ){
    return RowSelection();
  }

  const int largestIndex = *it;

  QStringList stringList;
  for(int i = 0; i <= largestIndex; ++i){
    stringList.append( QString::number(i) );
  }

  QStringListModel model(stringList);

  return makeRowSelectionFromIndexList(model, indexList);
}

std::vector<int> makeIndexListFromRowSelection(const RowSelection & selection) noexcept
{
  std::vector<int> indexList;
  RowListView listView(selection);

  std::copy( listView.cbegin(), listView.cend(), std::back_inserter(indexList) );

  return indexList;
}

void selectRowsAndColumn(QItemSelectionModel & selectionModel,
                         const std::vector<int> & rows, int column,
                         QItemSelectionModel::SelectionFlags command) noexcept
{
  QAbstractItemModel *model = selectionModel.model();
  assert(model != nullptr);

  QItemSelection selection;

  for(int row : rows){
    QModelIndex index = model->index(row, column);
    assert( index.isValid() );
    selection.select(index, index);
  }

  selectionModel.select(selection, command);
}

}}} // namespace Mdt{ namespace ItemModel{ namespace TestLib{
