// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2024-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#include <Mdt/ItemView/Helpers.h>
#include <QStringListModel>
#include <QListView>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QApplication>
#include <QTimer>
#include <iostream>
#include <cassert>

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  QStringListModel model({"A","B","C"});

  QListView view;
  view.setModel(&model);
  view.show();
  assert(view.selectionModel() != nullptr);

  QModelIndex index = model.index(0,0);
  assert( index.isValid() );

  view.selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);

  assert(model.rowCount() == 3);
  if( !Mdt::ItemView::removeSelectedRows(view) ){
    std::cerr << "Rome selected rows failed" << std::endl;
    return 1;
  }
  assert(model.rowCount() == 2);

  QTimer::singleShot(1000, &app, &QApplication::quit);

  return app.exec();
}
