// SPDX-License-Identifier: LGPL-3.0-or-later
/****************************************************************************************
 **
 ** MdtModelView
 ** Set of libraries extending the Qt model-view framework.
 **
 ** Copyright (C) 2011-2023 Philippe Steinmann.
 **
 *****************************************************************************************/
#ifndef MDT_ITEM_MODEL_ITEM_SELECTION_MODEL_H
#define MDT_ITEM_MODEL_ITEM_SELECTION_MODEL_H

#include "mdt_itemmodel_export.h"
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QModelIndex>

namespace Mdt{ namespace ItemModel{

  /*! \brief ItemSelectionModel extends QItemSelectionModel to be used in list and details view
   *
   * \section ItemSelectionModel_Introduction Introduction
   *
   * For some cases, we would like to present a list of items to the user.
   * Once he selected an item, more details are presented in the UI.
   * Those details could be edited. In those details, other list of items could also be present.
   *
   * As an example, when the details are about to be edited,
   * it shoud not be possible to select an other item in the list,
   * otherwise modifications could be lost or incoherent.
   *
   * The application logic should decide if its possible to change the current row in the list.
   *
   * If we use QItemSelectionModel, we have no control on the selected item.
   * The view changes the current index, and QItemSelectionModel::currentChanged()
   * (and related signals) are emitted.
   *
   * With ItemSelectionModel, setChangeCurrentRowAllowed() can be used to allow or not
   * to set the current item to a different row than the effectively current one.
   *
   * \section ItemSelectionModel_Example Example
   *
   * In this example, a list of devices is presented in the UI.
   * The editable details about the current device are also presented in the UI.
   *
   * The device list only presents a subset of the attributes of the device.
   * When the user selects a row, a device editor will fetch all details about the device.
   * For this, an ID is extracted from the model used to present the list of devices.
   *
   * When the user edits some attributes of the currently presented device,
   * the editor goes to some editing state.
   * In this state, some operations are possible, like save or cancel changes.
   * Other operations, like presenting another device, are not allowed
   * (otherwise the edited data will be lost).
   * The UI also has to reflect this state (by enabling/disabling actions, for example).
   * Also, selecting another device in the list should not be possible.
   *
   * \code
   * void MainWindow::setupDeviceEditorUi()
   * {
   *   mUi.deviceListTableView->setModel(&mDeviceListTableModel);
   *   // It's important to set the selection model after the model
   *   mDeviceListSelectionModel.setModel(&mDeviceListTableModel);
   *   mDeviceListSelectionModel.setCurrentIndexToFirstRowAfterReset(true);
   *   mUi.deviceListTableView->setSelectionModel(&mDeviceListSelectionModel);
   *
   *   connect(mDeviceListSelectionModel, &ItemSelectionModel::currentRowChanged, this, &MainWindow::setCurrentDeviceFromListViewRow);
   * }
   *
   * void MainWindow::setCurrentDeviceFromListViewRow(const QModelIndex & viewCurrent)
   * {
   *   // If we have proxy models, we should map to source
   *   const QModelIndex index = viewCurrent;
   *
   *   const int row = index.row();
   *   const int column = DeviceListTableModel::idColumn();
   *
   *   QVariant id;
   *
   *   // row can be out of bound when editing a new device
   *   if( modelRowAndColumnAreInRange(mListViewModel, row, column) ){
   *     id = getModelData(mListViewModel, row, column);
   *   }
   *
   *   mEditor.setCurrentDeviceById(id);
   * }
   * \endcode
   *
   * In above example, the editor will emit a signal when it has fetched the requested device.
   * In the UI, a slot will display the detailed data. This code is not shown here.
   * If given id does not refer to an existing device,
   * the editor will pass an empty set of view data as argument of its signal.
   * On error, it could pass question marks in the view data, and also add an error in some way.
   *
   * The editor has different states, like Visualizing, Editing, etc...
   * When its state changes, it will signal it to the UI.
   * The UI then reflects this state:
   * \code
   * void MainWindow::setEditorState(const EditorState & state)
   * {
   *   // Enable/disable different actions regarding given state
   *
   *   mDeviceListSelectionModel.setChangeCurrentRowAllowed( state.canChangeCurrentDevice() );
   * }
   * \endcode
   *
   *
   * \sa https://doc.qt.io/qt-6/qabstractitemview.html#setSelectionModel
   *
   * \section ItemSelectionModel_Rationale Rationale
   *
   * \todo identify signals, like model reset, insert, remove, to which QItemSelectionModel reacts,
   * and do the same.
   * Then, imagine a CurrentRowSelectionController
   *
   * \todo on insert, what should we do.
   * Current row will change, but, currently, changeCurrentRowRequested() is not emitted.
   * What should the app do with that ?
   * Think about filter proxy model, also sort proxy model
   *
   * \subsection ItemSelectionModel_Rationale_Design Design: Allow / disallow vs request / confirmation
   *
   * First version was based on a request / confirmation pattern.
   *
   * When the user wants to navigates to another row, a changeCurrentRowRequested() is emitted,
   * but the current index does not change.
   * The, the application can choose to confirm to go to the specified row
   * by calling doSetCurrentRow().
   *
   * The problem with that is a memorized requested row that is hold somewhere.
   * Things can happen to a list of items between the request and the confirmation.
   *
   * As an example, imagine a list of items presented with a QTableView.
   * The user can interactively sort the list:
   * - The user wants to see details for row is 1
   * - The changeCurrentRowRequested(1) has been emitted
   * - The applcation cannot fetch detailed data for the requested row,
   *   because some edited data has not been commited (will loose the changes)
   * - The user sorts the list
   * - The current row is now 9 , but the changeCurrentRowRequested() signal has not been emitted
   * - The user saves the currently edited detailed of the current item
   * - The application calls doSetCurrentRow(1)
   *
   * The application should not call doSetCurrentRow() at this late time,
   * it should simply discard the request and wait until a new row change request comes.
   * The point here is that this pattern is error prone.
   *
   * \subsection ItemSelectionModel_Rationale_CurrentChanged Current changed signals
   *
   * To try to understand when the currentChanged() signal is emitted by QItemSelectionModel,
   * I looked in the source code of Qt 5.14.2 and 6.2.3 .
   *
   * currentChanged() is emitted:
   * - when rows (also columns) are removed - But seems not always
   * - by calling clearCurrentIndex() also clear()
   * - by setCurrentIndex()
   *
   * It is NOT emitted when:
   * - model is reset (clearly documented)
   * - rows are inserted (not documented)
   *
   * There is a bug report, but it talks about selectionChanged(), NOT currentChanged():
   * https://bugreports.qt.io/browse/QTBUG-93305
   *
   * See the following examples to understand that the behavior is correct.
   *
   * \subsection ItemSelectionModel_Rationale_Sorting Sorting
   *
   * Here we try to answer the question:
   * should currentChanged() be emitted when rows moves because of sorting ?
   *
   * Start with a list presenting A,B ascending:
   * - row 0 presents A
   * - current row is 0 and is selected
   * - the user sorts descending
   * - current row is now 1 , it is selected and it presents A
   * - currentChanged() has NOT been emitted
   * - the user clicks on the same row 1
   * - currentChanged() is also not emitted
   *
   * This behavior is Ok.
   * It avoids to fetch exactliy the same content just because of a sort.
   * The current and selected item are correctly presented.
   *
   * The only thing that can go wrong is if the application
   * has stored the current index (or current row),
   * and it uses it after a sort.
   *
   * \subsection ItemSelectionModel_Rationale_RemoveRow Remove row
   *
   * Here we try to understand what happens when rows are removed.
   *
   * Starting with a list A,B:
   * - row 0 presents A
   * - current row is 0 and is selected
   * - user removes this row 0
   * - currentChanged() is emitted
   *   Its current argument contains row 1
   *   Stuff can be done here, like getting an Id at row 1 to fetch detailed data
   * - then, row 0 is removed
   * - current row is now 0.
   *   With QTableView and its default selection behavior, no item is selected,
   *   but current item is at row 0 (visible when the table view has the focus).
   *
   * This behavior is Ok.
   * It's important to understand that currentChanged() is emitted before the row removal.
   *
   * \subsection ItemSelectionModel_Rationale_Filtering Filtering
   *
   * Here we try to understand what to do when rows are inserted and removed.
   * This will happen when using a filter proxy model.
   *
   * \subsubsection ItemSelectionModel_Rationale_Filtering_RemoveCurrentRow Remove current row
   *
   * Start with a list presenting A,B:
   * - row 0 presents A
   * - current row is 0 and is selected
   * - the user filters to only see B
   * - currentChanged() is emitted.
   *   Its current index argument contains the row 1
   *   (which represents item B)
   * - row 0 is removed
   * - row 0 presents B
   * - row 0 is not selected
   *   (depends an view/selection mode. Here QTableView with default mode and behavior)
   * - current row is 0
   * - the user clicks on the same row 0
   * - currentChanged() is not emitted
   *   (it has not to be emitted)
   *
   * This behavior is Ok.
   * It's important to understand that currentChanged() is emitted before the row removal.
   * If some details are to be fetched, depending of some Id available in the model,
   * it will be fetched before the row removal.
   *
   * \subsubsection ItemSelectionModel_Rationale_Filtering_InsertBeforeCurrentRow Insert before current row
   *
   * Start with a list A,B , filtered on B:
   * - row 0 presents B
   * - current row is 0 and is selected
   * - the user filters to see all items
   * - row with item A is inserted before B
   * - currentChanged() is NOT emitted.
   * - current row is 1 and is selected
   * - row 1 presents B
   *
   * This behavior is Ok.
   * It avoids to fetch exactliy the same content just because of a filter.
   * The current and selected item are correctly presented.
   *
   * The only thing that can go wrong is if the application
   * has stored the current index (or current row),
   * and it uses it after a filter, or an insert due for another reason.
   *
   *
   * \todo async fetching (should block row change until fetched ?)
   */
  class MDT_ITEMMODEL_EXPORT ItemSelectionModel : public QItemSelectionModel
  {
   Q_OBJECT

   public:

    /*! \brief Constructor
     */
    ItemSelectionModel(QAbstractItemModel* model = nullptr);

    /*! \brief Constructor
     */
    ItemSelectionModel(QAbstractItemModel* model, QObject *parent);

    // Copy disabled
    ItemSelectionModel(const ItemSelectionModel &) = delete;
    ItemSelectionModel & operator=(const ItemSelectionModel &) = delete;
    // Move disabled
    ItemSelectionModel(ItemSelectionModel &&) = delete;
    ItemSelectionModel & operator=(ItemSelectionModel &&) = delete;

    /*! \brief Set the current index to the first row after model reset
     *
     * By default, the current index becomes invalid after a model reset.
     * Also, currentChanged() and related signals are not emitted after a model reset.
     *
     * If this option is enabled,
     * the current index will be set to the first row after a model reset.
     * If the model is empty after the reset, current index will be invalid.
     *
     * By default, currentChanged() and related signals are not emitted after a model reset.
     *
     * If this option is enabled,
     * currentChanged() and currentRowChanged() will be emitted
     * after a model reset with its \a current index referencing the first row.
     * If the model becomes empty, its \a current index will be invalid.
     */
    void setCurrentIndexToFirstRowAfterReset(bool enable) noexcept;

    /*! \brief Check if set the current index to the first row after model reset is enabled
     *
     * \sa setCurrentIndexToFirstRowAfterReset()
     */
    bool setCurrentIndexToFirstRowAfterResetIsEnabled() const noexcept
    {
      return mSetCurrentIndexToFirstRowAfterResetIsEnabled;
    }

    /*! \brief Check if changing current row is allowed
     *
     * \sa setChangeCurrentRowAllowed()
     */
    bool changeCurrentRowIsAllowed() const noexcept
    {
      return mChangeCurrentRowIsAllowed;
    }

    /*! \internal Check if current index can be set to the given one
     */
    bool canSetCurrentIndex(const QModelIndex & index) noexcept;

    /*! \internal Check if given selection can be applied
     */
    bool canSelect(const QItemSelection & selection) noexcept;

    /*! \internal Check if given range contains multiple items
     */
    static
    bool isMultipleItemsSelectionRange(const QItemSelectionRange & range) noexcept;

    /*! \internal Check if given selection contains multiple items
     */
    static
    bool isMultipleItemsSelection(const QItemSelection & selection) noexcept;

   public slots:

    /*! \brief Set changing current row allowed or not
     *
     * By default, changing current row is allowed.
     *
     * \sa changeCurrentRowIsAllowed()
     */
    void setChangeCurrentRowAllowed(bool allow) noexcept;

    /*! \brief Re-implemented to intercept row change
     *
     * \sa setCurrentIndex()
     * 
     * \todo QItemSelectionModel's implementation calls the other overload
     */
    void select(const QModelIndex & index, QItemSelectionModel::SelectionFlags command) override;

    /*! \brief Re-implemented to intercept row change
     *
     * \sa setCurrentIndex()
     */
    void select(const QItemSelection & selection, QItemSelectionModel::SelectionFlags command) override;

    /*! \brief Re-implemented to intercept row change
     */
    void setCurrentIndex(const QModelIndex & index, QItemSelectionModel::SelectionFlags command) override;

    /*! \brief Re-implemented to intercept model reset
     */
    void reset() override;

  private:

    bool isRowChangeRequest(int row) const noexcept;
    bool isRowChangeRequest(const QModelIndex & index) const noexcept;
    bool isRowChangeRequest(const QItemSelection & selection) const noexcept;

    bool mChangeCurrentRowIsAllowed = true;
    bool mSetCurrentIndexToFirstRowAfterResetIsEnabled = false;
  };

}} // namespace Mdt{ namespace ItemModel{

#endif // #ifndef MDT_ITEM_MODEL_ITEM_SELECTION_MODEL_H
