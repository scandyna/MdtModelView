// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2024 Philippe Steinmann.
 **
 *****************************************************************************************/
#include "catch2/catch.hpp"
#include "Catch2QString.h"
#include "Mdt/ItemModel/ProxyModelPipeline.h"
#include "Mdt/ItemModel/Helpers.h"
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QItemSelectionRange>
#include <QString>
#include <QLatin1String>
#include <QStringList>
#include <string>
#include <vector>

using namespace Mdt::ItemModel;

void populateModel(QStringListModel & model, const std::vector<std::string> & stringList)
{
  QStringList qStringList;

  for(const std::string & str : stringList){
    qStringList.append( QString::fromStdString(str) );
  }

  model.setStringList(qStringList);
}


TEST_CASE("onlySourceModel")
{
  ProxyModelPipeline pipeline;
  QStringListModel model;

  pipeline.setSourceModel(&model);

  REQUIRE( pipeline.modelForView() == &model );
}

TEST_CASE("appendProxyModel")
{
  ProxyModelPipeline pipeline;
  QStringListModel model;

  pipeline.setSourceModel(&model);

  SECTION("1 proxy model")
  {
    QSortFilterProxyModel filterModel;

    pipeline.appendProxyModel(&filterModel);

    REQUIRE( filterModel.sourceModel() == &model );
    REQUIRE( pipeline.modelForView() == &filterModel );
  }

  SECTION("2 proxy models")
  {
    QSortFilterProxyModel filterModel;
    QSortFilterProxyModel sortModel;

    pipeline.appendProxyModel(&filterModel);
    pipeline.appendProxyModel(&sortModel);

    REQUIRE( filterModel.sourceModel() == &model );
    REQUIRE( sortModel.sourceModel() == &filterModel );
    REQUIRE( pipeline.modelForView() == &sortModel );
  }
}

TEST_CASE("mapIndexToSource")
{
  ProxyModelPipeline pipeline;
  QStringListModel model;
  QAbstractItemModel *viewModel;

  populateModel(model, {"B","C","A"});
  pipeline.setSourceModel(&model);

  /*
   * |0||B|
   * |1||C|
   * |2||A|
   */
  SECTION("onlySourceModel")
  {
    viewModel = pipeline.modelForView();

    REQUIRE( viewModel->rowCount() == 3 );

    SECTION("view row 0 maps to source row 0")
    {
      QModelIndex viewIndex = viewModel->index(0, 0);
      QModelIndex modelIndex = pipeline.mapIndexToSource(viewIndex);

      REQUIRE( modelIndex.row() == 0 );
    }

    SECTION("view row 1 maps to source row 1")
    {
      QModelIndex viewIndex = viewModel->index(1, 0);
      QModelIndex modelIndex = pipeline.mapIndexToSource(viewIndex);

      REQUIRE( modelIndex.row() == 1 );
    }

    SECTION("view row 2 maps to source row 2")
    {
      QModelIndex viewIndex = viewModel->index(2, 0);
      QModelIndex modelIndex = pipeline.mapIndexToSource(viewIndex);

      REQUIRE( modelIndex.row() == 2 );
    }
  }

  /*
   * |0||B|
   * |1||C|  |0||C|
   * |2||A|  |1||A|
   */
  SECTION("filter proxy model")
  {
    QSortFilterProxyModel filterModel;
    pipeline.appendProxyModel(&filterModel);
    viewModel = pipeline.modelForView();
    filterModel.setFilterRegularExpression( QLatin1String("C|A") );

    REQUIRE( viewModel->rowCount() == 2 );
    REQUIRE( getModelData(*viewModel, 0, 0) == QLatin1String("C") );
    REQUIRE( getModelData(*viewModel, 1, 0) == QLatin1String("A") );

    SECTION("view row 0 maps to source row 1")
    {
      QModelIndex viewIndex = viewModel->index(0, 0);
      QModelIndex modelIndex = pipeline.mapIndexToSource(viewIndex);

      REQUIRE( modelIndex.row() == 1 );
    }

    SECTION("view row 1 maps to source row 2")
    {
      QModelIndex viewIndex = viewModel->index(1, 0);
      QModelIndex modelIndex = pipeline.mapIndexToSource(viewIndex);

      REQUIRE( modelIndex.row() == 2 );
    }
  }

  /*
   * |0||B|  |0||A|
   * |1||C|  |1||B|
   * |2||A|  |2||C|
   */
  SECTION("sort proxy model")
  {
    QSortFilterProxyModel sortModel;
    pipeline.appendProxyModel(&sortModel);
    viewModel = pipeline.modelForView();
    sortModel.sort(0);

    REQUIRE( viewModel->rowCount() == 3 );
    REQUIRE( getModelData(*viewModel, 0, 0) == QLatin1String("A") );
    REQUIRE( getModelData(*viewModel, 1, 0) == QLatin1String("B") );
    REQUIRE( getModelData(*viewModel, 2, 0) == QLatin1String("C") );

    SECTION("view row 0 maps to source row 2")
    {
      QModelIndex viewIndex = viewModel->index(0, 0);
      QModelIndex modelIndex = pipeline.mapIndexToSource(viewIndex);

      REQUIRE( modelIndex.row() == 2 );
    }

    SECTION("view row 1 maps to source row 0")
    {
      QModelIndex viewIndex = viewModel->index(1, 0);
      QModelIndex modelIndex = pipeline.mapIndexToSource(viewIndex);

      REQUIRE( modelIndex.row() == 0 );
    }

    SECTION("view row 2 maps to source row 1")
    {
      QModelIndex viewIndex = viewModel->index(2, 0);
      QModelIndex modelIndex = pipeline.mapIndexToSource(viewIndex);

      REQUIRE( modelIndex.row() == 1 );
    }
  }

  /*
   * |0||B|
   * |1||C|  |0||C|  |0||A|
   * |2||A|  |1||A|  |1||C|
   */
  SECTION("filter and sort proxy models")
  {
    QSortFilterProxyModel filterModel;
    QSortFilterProxyModel sortModel;
    pipeline.appendProxyModel(&filterModel);
    pipeline.appendProxyModel(&sortModel);
    viewModel = pipeline.modelForView();
    filterModel.setFilterRegularExpression( QLatin1String("C|A") );
    sortModel.sort(0);

    REQUIRE( viewModel->rowCount() == 2 );
    REQUIRE( getModelData(*viewModel, 0, 0) == QLatin1String("A") );
    REQUIRE( getModelData(*viewModel, 1, 0) == QLatin1String("C") );

    SECTION("view row 0 maps to source row 2")
    {
      QModelIndex viewIndex = viewModel->index(0, 0);
      QModelIndex modelIndex = pipeline.mapIndexToSource(viewIndex);

      REQUIRE( modelIndex.row() == 2 );
    }

    SECTION("view row 1 maps to source row 1")
    {
      QModelIndex viewIndex = viewModel->index(1, 0);
      QModelIndex modelIndex = pipeline.mapIndexToSource(viewIndex);

      REQUIRE( modelIndex.row() == 1 );
    }
  }
}

TEST_CASE("mapIndexFromSource")
{
  ProxyModelPipeline pipeline;
  QStringListModel sourceModel;
  QAbstractItemModel *viewModel;

  populateModel(sourceModel, {"B","C","A"});
  pipeline.setSourceModel(&sourceModel);

  /*
   * |0||B|
   * |1||C|
   * |2||A|
   */
  SECTION("onlySourceModel")
  {
    viewModel = pipeline.modelForView();

    REQUIRE( viewModel->rowCount() == 3 );

    SECTION("source row 0 maps to view row 0")
    {
      QModelIndex sourceIndex = sourceModel.index(0, 0);
      QModelIndex viewIndex = pipeline.mapIndexFromSource(sourceIndex);

      REQUIRE( viewIndex.row() == 0 );
    }

    SECTION("source row 1 maps to view row 1")
    {
      QModelIndex sourceIndex = sourceModel.index(1, 0);
      QModelIndex viewIndex = pipeline.mapIndexFromSource(sourceIndex);

      REQUIRE( viewIndex.row() == 1 );
    }

    SECTION("source row 2 maps to view row 2")
    {
      QModelIndex sourceIndex = sourceModel.index(2, 0);
      QModelIndex viewIndex = pipeline.mapIndexFromSource(sourceIndex);

      REQUIRE( viewIndex.row() == 2 );
    }
  }

  /*
   * |0||B|
   * |1||C|  |0||C|
   * |2||A|  |1||A|
   */
  SECTION("filter proxy model")
  {
    QSortFilterProxyModel filterModel;
    pipeline.appendProxyModel(&filterModel);
    viewModel = pipeline.modelForView();
    filterModel.setFilterRegularExpression( QLatin1String("C|A") );

    REQUIRE( viewModel->rowCount() == 2 );
    REQUIRE( getModelData(*viewModel, 0, 0) == QLatin1String("C") );
    REQUIRE( getModelData(*viewModel, 1, 0) == QLatin1String("A") );

    SECTION("source row 1 maps to view row 0")
    {
      QModelIndex sourceIndex = sourceModel.index(1, 0);
      QModelIndex viewIndex = pipeline.mapIndexFromSource(sourceIndex);

      REQUIRE( viewIndex.row() == 0 );
    }

    SECTION("source row 2 maps to view row 1")
    {
      QModelIndex sourceIndex = sourceModel.index(2, 0);
      QModelIndex viewIndex = pipeline.mapIndexFromSource(sourceIndex);

      REQUIRE( viewIndex.row() == 1 );
    }

    SECTION("source row 0 maps to no view row")
    {
      QModelIndex sourceIndex = sourceModel.index(0, 0);
      QModelIndex viewIndex = pipeline.mapIndexFromSource(sourceIndex);

      REQUIRE( !viewIndex.isValid() );
    }
  }

  /*
   * |0||B|  |0||A|
   * |1||C|  |1||B|
   * |2||A|  |2||C|
   */
  SECTION("sort proxy model")
  {
    QSortFilterProxyModel sortModel;
    pipeline.appendProxyModel(&sortModel);
    viewModel = pipeline.modelForView();
    sortModel.sort(0);

    REQUIRE( viewModel->rowCount() == 3 );
    REQUIRE( getModelData(*viewModel, 0, 0) == QLatin1String("A") );
    REQUIRE( getModelData(*viewModel, 1, 0) == QLatin1String("B") );
    REQUIRE( getModelData(*viewModel, 2, 0) == QLatin1String("C") );

    SECTION("source row 2 maps to view row 0")
    {
      QModelIndex sourceIndex = sourceModel.index(2, 0);
      QModelIndex viewIndex = pipeline.mapIndexFromSource(sourceIndex);

      REQUIRE( viewIndex.row() == 0 );
    }

    SECTION("source row 0 maps to view row 1")
    {
      QModelIndex sourceIndex = sourceModel.index(0, 0);
      QModelIndex viewIndex = pipeline.mapIndexFromSource(sourceIndex);

      REQUIRE( viewIndex.row() == 1 );
    }

    SECTION("source row 1 maps to view row 2")
    {
      QModelIndex sourceIndex = sourceModel.index(1, 0);
      QModelIndex viewIndex = pipeline.mapIndexFromSource(sourceIndex);

      REQUIRE( viewIndex.row() == 2 );
    }
  }

  /*
   * |0||B|
   * |1||C|  |0||C|  |0||A|
   * |2||A|  |1||A|  |1||C|
   */
  SECTION("filter and sort proxy models")
  {
    QSortFilterProxyModel filterModel;
    QSortFilterProxyModel sortModel;
    pipeline.appendProxyModel(&filterModel);
    pipeline.appendProxyModel(&sortModel);
    viewModel = pipeline.modelForView();
    filterModel.setFilterRegularExpression( QLatin1String("C|A") );
    sortModel.sort(0);

    REQUIRE( viewModel->rowCount() == 2 );
    REQUIRE( getModelData(*viewModel, 0, 0) == QLatin1String("A") );
    REQUIRE( getModelData(*viewModel, 1, 0) == QLatin1String("C") );

    SECTION("source row 2 maps to view row 0")
    {
      QModelIndex sourceIndex = sourceModel.index(2, 0);
      QModelIndex viewIndex = pipeline.mapIndexFromSource(sourceIndex);

      REQUIRE( viewIndex.row() == 0 );
    }

    SECTION("source row 1 maps to view row 1")
    {
      QModelIndex sourceIndex = sourceModel.index(1, 0);
      QModelIndex viewIndex = pipeline.mapIndexFromSource(sourceIndex);

      REQUIRE( viewIndex.row() == 1 );
    }

    SECTION("source row 0 maps to no view row")
    {
      QModelIndex sourceIndex = sourceModel.index(0, 0);
      QModelIndex viewIndex = pipeline.mapIndexFromSource(sourceIndex);

      REQUIRE( !viewIndex.isValid() );
    }
  }
}

TEST_CASE("mapSelectionToSource")
{
  ProxyModelPipeline pipeline;
  QStringListModel model;
  QAbstractItemModel *viewModel;

  populateModel(model, {"B","C","A"});
  pipeline.setSourceModel(&model);

  /*
   * |0||B|
   * |1||C|
   * |2||A|
   */
  SECTION("onlySourceModel")
  {
    viewModel = pipeline.modelForView();

    REQUIRE( viewModel->rowCount() == 3 );

    SECTION("selecting row 0 in view maps to source row 0")
    {
      QModelIndex viewIndex = viewModel->index(0, 0);
      QItemSelection viewSelection(viewIndex, viewIndex);
      QItemSelection modelSelection = pipeline.mapSelectionToSource(viewSelection);

      REQUIRE( modelSelection.count() == 1 );
      REQUIRE( modelSelection.at(0).top() == 0 );
      REQUIRE( modelSelection.at(0).bottom() == 0 );
    }

    SECTION("selecting row 1 in view maps to source row 1")
    {
      QModelIndex viewIndex = viewModel->index(1, 0);
      QItemSelection viewSelection(viewIndex, viewIndex);
      QItemSelection modelSelection = pipeline.mapSelectionToSource(viewSelection);

      REQUIRE( modelSelection.count() == 1 );
      REQUIRE( modelSelection.at(0).top() == 1 );
      REQUIRE( modelSelection.at(0).bottom() == 1 );
    }

    SECTION("selecting row 2 in view maps to source row 2")
    {
      QModelIndex viewIndex = viewModel->index(2, 0);
      QItemSelection viewSelection(viewIndex, viewIndex);
      QItemSelection modelSelection = pipeline.mapSelectionToSource(viewSelection);

      REQUIRE( modelSelection.count() == 1 );
      REQUIRE( modelSelection.at(0).top() == 2 );
      REQUIRE( modelSelection.at(0).bottom() == 2 );
    }
  }

  /*
   * |0||B|
   * |1||C|  |0||C|
   * |2||A|  |1||A|
   */
  SECTION("filter proxy model")
  {
    QSortFilterProxyModel filterModel;
    pipeline.appendProxyModel(&filterModel);
    viewModel = pipeline.modelForView();
    filterModel.setFilterRegularExpression( QLatin1String("C|A") );

    REQUIRE( viewModel->rowCount() == 2 );
    REQUIRE( getModelData(*viewModel, 0, 0) == QLatin1String("C") );
    REQUIRE( getModelData(*viewModel, 1, 0) == QLatin1String("A") );

    SECTION("selecting row 0 in view maps to source row 1")
    {
      QModelIndex viewIndex = viewModel->index(0, 0);
      QItemSelection viewSelection(viewIndex, viewIndex);
      QItemSelection modelSelection = pipeline.mapSelectionToSource(viewSelection);

      REQUIRE( modelSelection.count() == 1 );
      REQUIRE( modelSelection.at(0).top() == 1 );
      REQUIRE( modelSelection.at(0).bottom() == 1 );
    }

    SECTION("selecting row 1 in view maps to source row 2")
    {
      QModelIndex viewIndex = viewModel->index(1, 0);
      QItemSelection viewSelection(viewIndex, viewIndex);
      QItemSelection modelSelection = pipeline.mapSelectionToSource(viewSelection);

      REQUIRE( modelSelection.count() == 1 );
      REQUIRE( modelSelection.at(0).top() == 2 );
      REQUIRE( modelSelection.at(0).bottom() == 2 );
    }
  }

  /*
   * |0||B|  |0||A|
   * |1||C|  |1||B|
   * |2||A|  |2||C|
   */
  SECTION("sort proxy model")
  {
    QSortFilterProxyModel sortModel;
    pipeline.appendProxyModel(&sortModel);
    viewModel = pipeline.modelForView();
    sortModel.sort(0);

    REQUIRE( viewModel->rowCount() == 3 );
    REQUIRE( getModelData(*viewModel, 0, 0) == QLatin1String("A") );
    REQUIRE( getModelData(*viewModel, 1, 0) == QLatin1String("B") );
    REQUIRE( getModelData(*viewModel, 2, 0) == QLatin1String("C") );

    SECTION("selecting row 0 in view maps to source row 2")
    {
      QModelIndex viewIndex = viewModel->index(0, 0);
      QItemSelection viewSelection(viewIndex, viewIndex);
      QItemSelection modelSelection = pipeline.mapSelectionToSource(viewSelection);

      REQUIRE( modelSelection.count() == 1 );
      REQUIRE( modelSelection.at(0).top() == 2 );
      REQUIRE( modelSelection.at(0).bottom() == 2 );
    }

    SECTION("selecting row 1 in view maps to source row 0")
    {
      QModelIndex viewIndex = viewModel->index(1, 0);
      QItemSelection viewSelection(viewIndex, viewIndex);
      QItemSelection modelSelection = pipeline.mapSelectionToSource(viewSelection);

      REQUIRE( modelSelection.count() == 1 );
      REQUIRE( modelSelection.at(0).top() == 0 );
      REQUIRE( modelSelection.at(0).bottom() == 0 );
    }

    SECTION("selecting row 2 in view maps to source row 1")
    {
      QModelIndex viewIndex = viewModel->index(2, 0);
      QItemSelection viewSelection(viewIndex, viewIndex);
      QItemSelection modelSelection = pipeline.mapSelectionToSource(viewSelection);

      REQUIRE( modelSelection.count() == 1 );
      REQUIRE( modelSelection.at(0).top() == 1 );
      REQUIRE( modelSelection.at(0).bottom() == 1 );
    }
  }

  /*
   * |0||B|
   * |1||C|  |0||C|  |0||A|
   * |2||A|  |1||A|  |1||C|
   */
  SECTION("filter and sort proxy models")
  {
    QSortFilterProxyModel filterModel;
    QSortFilterProxyModel sortModel;
    pipeline.appendProxyModel(&filterModel);
    pipeline.appendProxyModel(&sortModel);
    viewModel = pipeline.modelForView();
    filterModel.setFilterRegularExpression( QLatin1String("C|A") );
    sortModel.sort(0);

    REQUIRE( viewModel->rowCount() == 2 );
    REQUIRE( getModelData(*viewModel, 0, 0) == QLatin1String("A") );
    REQUIRE( getModelData(*viewModel, 1, 0) == QLatin1String("C") );

    SECTION("selecting row 0 in view maps to source row 2")
    {
      QModelIndex viewIndex = viewModel->index(0, 0);
      QItemSelection viewSelection(viewIndex, viewIndex);
      QItemSelection modelSelection = pipeline.mapSelectionToSource(viewSelection);

      REQUIRE( modelSelection.count() == 1 );
      REQUIRE( modelSelection.at(0).top() == 2 );
      REQUIRE( modelSelection.at(0).bottom() == 2 );
    }

    SECTION("selecting row 1 in view maps to source row 1")
    {
      QModelIndex viewIndex = viewModel->index(1, 0);
      QItemSelection viewSelection(viewIndex, viewIndex);
      QItemSelection modelSelection = pipeline.mapSelectionToSource(viewSelection);

      REQUIRE( modelSelection.count() == 1 );
      REQUIRE( modelSelection.at(0).top() == 1 );
      REQUIRE( modelSelection.at(0).bottom() == 1 );
    }
  }
}

TEST_CASE("mapSelectionFromSource")
{
  ProxyModelPipeline pipeline;
  QStringListModel sourceModel;
  QAbstractItemModel *viewModel;

  populateModel(sourceModel, {"B","C","A"});
  pipeline.setSourceModel(&sourceModel);

  /*
   * |0||B|
   * |1||C|
   * |2||A|
   */
  SECTION("onlySourceModel")
  {
    viewModel = pipeline.modelForView();

    REQUIRE( viewModel->rowCount() == 3 );

    SECTION("selecting row 0 in source maps to view row 0")
    {
      QModelIndex sourceIndex = sourceModel.index(0, 0);
      QItemSelection sourceSelection(sourceIndex, sourceIndex);

      QItemSelection viewSelection = pipeline.mapSelectionFromSource(sourceSelection);

      REQUIRE( viewSelection.count() == 1 );
      REQUIRE( viewSelection.at(0).top() == 0 );
      REQUIRE( viewSelection.at(0).bottom() == 0 );
    }

    SECTION("selecting row 1 in source maps to view row 1")
    {
      QModelIndex sourceIndex = sourceModel.index(1, 0);
      QItemSelection sourceSelection(sourceIndex, sourceIndex);

      QItemSelection viewSelection = pipeline.mapSelectionFromSource(sourceSelection);

      REQUIRE( viewSelection.count() == 1 );
      REQUIRE( viewSelection.at(0).top() == 1 );
      REQUIRE( viewSelection.at(0).bottom() == 1 );
    }

    SECTION("selecting row 2 in source maps to view row 2")
    {
      QModelIndex sourceIndex = sourceModel.index(2, 0);
      QItemSelection sourceSelection(sourceIndex, sourceIndex);

      QItemSelection viewSelection = pipeline.mapSelectionFromSource(sourceSelection);

      REQUIRE( viewSelection.count() == 1 );
      REQUIRE( viewSelection.at(0).top() == 2 );
      REQUIRE( viewSelection.at(0).bottom() == 2 );
    }
  }

  /*
   * |0||B|
   * |1||C|  |0||C|
   * |2||A|  |1||A|
   */
  SECTION("filter proxy model")
  {
    QSortFilterProxyModel filterModel;
    pipeline.appendProxyModel(&filterModel);
    viewModel = pipeline.modelForView();
    filterModel.setFilterRegularExpression( QLatin1String("C|A") );

    REQUIRE( viewModel->rowCount() == 2 );
    REQUIRE( getModelData(*viewModel, 0, 0) == QLatin1String("C") );
    REQUIRE( getModelData(*viewModel, 1, 0) == QLatin1String("A") );

    SECTION("selecting row 0 in source maps to an empty view selection")
    {
      QModelIndex sourceIndex = sourceModel.index(0, 0);
      QItemSelection sourceSelection(sourceIndex, sourceIndex);

      QItemSelection viewSelection = pipeline.mapSelectionFromSource(sourceSelection);

      REQUIRE( viewSelection.isEmpty() );
    }

    SECTION("selecting row 1 in source maps to view row 0")
    {
      QModelIndex sourceIndex = sourceModel.index(1, 0);
      QItemSelection sourceSelection(sourceIndex, sourceIndex);

      QItemSelection viewSelection = pipeline.mapSelectionFromSource(sourceSelection);

      REQUIRE( viewSelection.count() == 1 );
      REQUIRE( viewSelection.at(0).top() == 0 );
      REQUIRE( viewSelection.at(0).bottom() == 0 );
    }

    SECTION("selecting row 2 in source maps to view row 1")
    {
      QModelIndex sourceIndex = sourceModel.index(2, 0);
      QItemSelection sourceSelection(sourceIndex, sourceIndex);

      QItemSelection viewSelection = pipeline.mapSelectionFromSource(sourceSelection);

      REQUIRE( viewSelection.count() == 1 );
      REQUIRE( viewSelection.at(0).top() == 1 );
      REQUIRE( viewSelection.at(0).bottom() == 1 );
    }
  }

  /*
   * |0||B|  |0||A|
   * |1||C|  |1||B|
   * |2||A|  |2||C|
   */
  SECTION("sort proxy model")
  {
    QSortFilterProxyModel sortModel;
    pipeline.appendProxyModel(&sortModel);
    viewModel = pipeline.modelForView();
    sortModel.sort(0);

    REQUIRE( viewModel->rowCount() == 3 );
    REQUIRE( getModelData(*viewModel, 0, 0) == QLatin1String("A") );
    REQUIRE( getModelData(*viewModel, 1, 0) == QLatin1String("B") );
    REQUIRE( getModelData(*viewModel, 2, 0) == QLatin1String("C") );

    SECTION("selecting row 0 in source maps to view row 1")
    {
      QModelIndex sourceIndex = sourceModel.index(0, 0);
      QItemSelection sourceSelection(sourceIndex, sourceIndex);

      QItemSelection viewSelection = pipeline.mapSelectionFromSource(sourceSelection);

      REQUIRE( viewSelection.count() == 1 );
      REQUIRE( viewSelection.at(0).top() == 1 );
      REQUIRE( viewSelection.at(0).bottom() == 1 );
    }

    SECTION("selecting row 1 in source maps to view row 2")
    {
      QModelIndex sourceIndex = sourceModel.index(1, 0);
      QItemSelection sourceSelection(sourceIndex, sourceIndex);

      QItemSelection viewSelection = pipeline.mapSelectionFromSource(sourceSelection);

      REQUIRE( viewSelection.count() == 1 );
      REQUIRE( viewSelection.at(0).top() == 2 );
      REQUIRE( viewSelection.at(0).bottom() == 2 );
    }

    SECTION("selecting row 2 in source maps to view row 0")
    {
      QModelIndex sourceIndex = sourceModel.index(2, 0);
      QItemSelection sourceSelection(sourceIndex, sourceIndex);

      QItemSelection viewSelection = pipeline.mapSelectionFromSource(sourceSelection);

      REQUIRE( viewSelection.count() == 1 );
      REQUIRE( viewSelection.at(0).top() == 0 );
      REQUIRE( viewSelection.at(0).bottom() == 0 );
    }
  }

  /*
   * |0||B|
   * |1||C|  |0||C|  |0||A|
   * |2||A|  |1||A|  |1||C|
   */
  SECTION("filter and sort proxy models")
  {
    QSortFilterProxyModel filterModel;
    QSortFilterProxyModel sortModel;
    pipeline.appendProxyModel(&filterModel);
    pipeline.appendProxyModel(&sortModel);
    viewModel = pipeline.modelForView();
    filterModel.setFilterRegularExpression( QLatin1String("C|A") );
    sortModel.sort(0);

    REQUIRE( viewModel->rowCount() == 2 );
    REQUIRE( getModelData(*viewModel, 0, 0) == QLatin1String("A") );
    REQUIRE( getModelData(*viewModel, 1, 0) == QLatin1String("C") );

    SECTION("selecting row 0 in source maps to an empty view selection")
    {
      QModelIndex sourceIndex = sourceModel.index(0, 0);
      QItemSelection sourceSelection(sourceIndex, sourceIndex);

      QItemSelection viewSelection = pipeline.mapSelectionFromSource(sourceSelection);

      REQUIRE( viewSelection.isEmpty() );
    }

    SECTION("selecting row 1 in source maps to view row 1")
    {
      QModelIndex sourceIndex = sourceModel.index(1, 0);
      QItemSelection sourceSelection(sourceIndex, sourceIndex);

      QItemSelection viewSelection = pipeline.mapSelectionFromSource(sourceSelection);

      REQUIRE( viewSelection.count() == 1 );
      REQUIRE( viewSelection.at(0).top() == 1 );
      REQUIRE( viewSelection.at(0).bottom() == 1 );
    }

    SECTION("selecting row 2 in source maps to view row 0")
    {
      QModelIndex sourceIndex = sourceModel.index(2, 0);
      QItemSelection sourceSelection(sourceIndex, sourceIndex);

      QItemSelection viewSelection = pipeline.mapSelectionFromSource(sourceSelection);

      REQUIRE( viewSelection.count() == 1 );
      REQUIRE( viewSelection.at(0).top() == 0 );
      REQUIRE( viewSelection.at(0).bottom() == 0 );
    }
  }
}
