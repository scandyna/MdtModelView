// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_PROXY_MODEL_PIPELINE_H
#define MDT_ITEM_MODEL_PROXY_MODEL_PIPELINE_H

#include "mdt_itemmodel_export.h"
#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QItemSelection>
#include <QModelIndex>
#include <QPointer>
#include <vector>
#include <cassert>

namespace Mdt{ namespace ItemModel{

  /*! \brief Pipeline of proxy models based on QAbstractProxyModel
   *
   * Assuming we have a model, a view and some proxy models:
   * \code
   * auto *view = new QTableView(this);
   * MyTableModel myModel;
   * FilterProxyModel filterModel;
   * SortProxyModel sortModel;
   * \endcode
   *
   * The proxy models can be added to a proxy model pipeline:
   * \code
   * ProxyModelPipeline modelPipeline;
   *
   * modelPipeline.setSourceModel(&myModel);
   * modelPipeline.appendProxyModel(&filterModel);
   * modelPipeline.appendProxyModel(&sortModel);
   * view->setModel( modelPipeline.modelForView() );
   * \endcode
   *
   * The first proxy model, \a filterModel , will get the data from the source model, \a myModel .
   * The second proxy model, \a sortModel , will ge the data from the first one, \a filterModel .
   * The view will get the data from the last appended proxy model, \a sortModel .
   *
   * This is the equivalent code without using a proxy model pipeline:
   * \code
   * filterModel.setSourceModel(&myModel);
   * sortModel.setSourceModel(&filterModel);
   * view->setModel(&sortModel);
   * \endcode
   *
   * \note The model pipeline does not manage the lifetime of any proxy model nor the source model.
   *
   * Sometimes we have to map an index from the view to the source model.
   * As an example, the row in the source model corresponds to an index in the data structure this model presents.
   *
   * The view index could be the current one:
   * \code
   * QModelIndex viewIndex = view->selectionModel()->currentIndex();
   * \endcode
   *
   * Then it can be mapped to the source model index:
   * \code
   * QModelIndex myModelIndex = modelPipeline.mapIndexToSource(viewIndex);
   * \endcode
   *
   * This is the equivalent code without using a proxy model pipeline:
   * \code
   * QModelIndex filterModelIndex = sortModel.mapToSource(viewIndex);
   * QModelIndex myModelIndex = filterModel.mapToSource(filterModelIndex);
   * \endcode
   *
   * Notice that we have to remember how we did the setup.
   * If we add another proxy model later, we have to remember to also adapt this part of the code.
   *
   * Similar to the previous example with model indexes,
   * it is also possible to map a selection to the source model.
   *
   * Here we obtain the selection:
   * \code
   * QItemSelection viewSelection = view->selectionModel()->selection();
   * \endcode
   *
   * Then it can be mapped to a selection in the source model:
   * \code
   * QItemSelection myModelSelection = modelPipeline.mapSelectionToSource(viewSelection);
   * \endcode
   *
   * This is the equivalent code without using a proxy model pipeline:
   * \code
   * QItemSelection filterModelSelection = sortModel.mapSelectionToSource(viewSelection);
   * QItemSelection myModelSelection = filterModel.mapSelectionToSource(filterModelSelection);
   * \endcode
   *
   * As before, we have to remember the proxy models setup
   * and adapt this part of the code if we change it.
   *
   * \sa \ref ItemModel_ContainerExample
   */
  class MDT_ITEMMODEL_EXPORT ProxyModelPipeline
  {
   public:

    /*! \brief Set source model
     *
     * \pre \a model must be a valid pointer
     * \pre no proxy model must have been added
     */
    void setSourceModel(QAbstractItemModel *model) noexcept;

    /*! \brief Add a proxy model to the end
     *
     * \pre this pipeline must at least reference a source model
     * \pre \a model must be a valid pointer
     * \sa setSourceModel()
     */
    void appendProxyModel(QAbstractProxyModel *model) noexcept;

    /*! \brief Get the model for the view
     *
     * If no proxy model have been added,
     * the source model will be returned.
     *
     * \pre this pipeline must at least reference a source model
     * \sa setSourceModel()
     */
    QAbstractItemModel *modelForView() const noexcept
    {
      assert( !mSourceModel.isNull() );

      if( mProxyModelList.empty() ){
        return mSourceModel;
      }

      return mProxyModelList.back();
    }

    /*! \brief Map given index to the source model index
     *
     * \pre this pipeline must at least reference a source model
     * \sa setSourceModel()
     */
    QModelIndex mapIndexToSource(const QModelIndex & viewIndex) const noexcept;

    /*! \brief Map given index from source model index
     *
     * Note that an invalid index can be returned.
     * This is the case, for example, with a filter proxy model,
     * when a source index is not displayed in the view.
     *
     * \pre this pipeline must at least reference a source model
     * \sa setSourceModel()
     */
    QModelIndex mapIndexFromSource(const QModelIndex & sourceIndex) const noexcept;

    /*! \brief Map given selection to the source model selection
     *
     * \pre this pipeline must at least reference a source model
     * \sa setSourceModel()
     */
    QItemSelection mapSelectionToSource(const QItemSelection & viewSelection) const noexcept;

    /*! \brief Map given selection from source model selection
     *
     * Note that an empty selection can be returned.
     * This is the case, for example, with a filter proxy model,
     * when a source selection is not displayed in the view.
     *
     * \pre this pipeline must at least reference a source model
     * \sa setSourceModel()
     */
    QItemSelection mapSelectionFromSource(const QItemSelection & sourceSelection) const noexcept;

   private:

    QPointer<QAbstractItemModel> mSourceModel;
    std::vector< QPointer<QAbstractProxyModel> > mProxyModelList;
  };

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_PROXY_MODEL_PIPELINE_H
