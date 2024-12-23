// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2023-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ListAndDetailViewWidget.h"
#include "DeviceLibrary.h"
#include <QApplication>
#include <memory>

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  auto deviceLibrary = std::make_shared<DeviceLibrary>();

  ListAndDetailViewWidget widget(deviceLibrary);
  widget.show();

  return app.exec();
}
