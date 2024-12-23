// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ItemSelectionModelTestWidget.h"
#include <QApplication>

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  ItemSelectionModelTestWidget widget;
  widget.show();

  return app.exec();
}
