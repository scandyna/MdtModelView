// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "RowSelectionHelpers.h"
#include <cassert>

namespace Mdt{ namespace ItemModel{

RowRange rowRangeFromItemSelectionRange(const QItemSelectionRange & itemSelectionRange) noexcept
{
  assert( itemSelectionRange.isValid() );
  assert( !itemSelectionRange.isEmpty() );

  return RowRange::fromFirstAndLastRow( itemSelectionRange.top(), itemSelectionRange.bottom() );
}

}} // namespace Mdt{ namespace ItemModel{
