// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "ProxyModelPipeline.h"
#include <algorithm>

namespace Mdt{ namespace ItemModel{

void ProxyModelPipeline::setSourceModel(QAbstractItemModel *model) noexcept
{
  assert( model != nullptr );
  assert( mProxyModelList.empty() );

  mSourceModel = model;
}

void ProxyModelPipeline::appendProxyModel(QAbstractProxyModel *model) noexcept
{
  assert( model != nullptr );
  assert( !mSourceModel.isNull() );

  if( mProxyModelList.empty() ){
    model->setSourceModel(mSourceModel);
  }else{
    model->setSourceModel( mProxyModelList.back() );
  }

  mProxyModelList.push_back(model);
}

QModelIndex ProxyModelPipeline::mapIndexToSource(const QModelIndex & viewIndex) const noexcept
{
  assert( !mSourceModel.isNull() );

  if( mProxyModelList.empty() ){
    return viewIndex;
  }

  QModelIndex index = viewIndex;

  const auto mapIndex = [&index](const QPointer<QAbstractProxyModel> & proxyModel){
    assert( !proxyModel.isNull() );
    index = proxyModel->mapToSource(index);
  };

  std::for_each(mProxyModelList.crbegin(), mProxyModelList.crend(), mapIndex);

  return index;
}

QModelIndex ProxyModelPipeline::mapIndexFromSource(const QModelIndex & sourceIndex) const noexcept
{
  assert( !mSourceModel.isNull() );
  assert(sourceIndex.model() == mSourceModel);

  if( mProxyModelList.empty() ){
    return sourceIndex;
  }

  QModelIndex index = sourceIndex;

  const auto mapIndex = [&index](const QPointer<QAbstractProxyModel> & proxyModel){
    assert( !proxyModel.isNull() );
    index = proxyModel->mapFromSource(index);
  };

  std::for_each(mProxyModelList.cbegin(), mProxyModelList.cend(), mapIndex);

  return index;
}

QItemSelection ProxyModelPipeline::mapSelectionToSource(const QItemSelection & viewSelection) const noexcept
{
  assert( !mSourceModel.isNull() );

  if( mProxyModelList.empty() ){
    return viewSelection;
  }

  QItemSelection selection = viewSelection;

  const auto mapSelection = [&selection](const QPointer<QAbstractProxyModel> & proxyModel){
    assert( !proxyModel.isNull() );
    selection = proxyModel->mapSelectionToSource(selection);
  };

  std::for_each(mProxyModelList.crbegin(), mProxyModelList.crend(), mapSelection);

  return selection;
}

QItemSelection ProxyModelPipeline::mapSelectionFromSource(const QItemSelection & sourceSelection) const noexcept
{
  assert( !mSourceModel.isNull() );

  if( mProxyModelList.empty() ){
    return sourceSelection;
  }

  QItemSelection selection = sourceSelection;

  const auto mapSelection = [&selection](const QPointer<QAbstractProxyModel> & proxyModel){
    assert( !proxyModel.isNull() );
    selection = proxyModel->mapSelectionFromSource(selection);
  };

  std::for_each(mProxyModelList.cbegin(), mProxyModelList.cend(), mapSelection);

  return selection;
}

}} // namespace Mdt{ namespace ItemModel{
