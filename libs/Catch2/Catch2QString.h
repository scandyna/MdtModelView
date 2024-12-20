// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2024-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_CATCH2_QSTRING_H
#define MDT_CATCH2_QSTRING_H

#include <QString>
#include <ostream>

std::ostream & operator <<(std::ostream & os, const QString & str);

#endif // #ifndef MDT_CATCH2_QSTRING_H
