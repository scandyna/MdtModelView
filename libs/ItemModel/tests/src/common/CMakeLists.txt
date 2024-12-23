# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

add_library(Mdt_ItemModelTestCommon STATIC
  ReadOnlyTableModel.cpp
  CustomRolesTableModel.cpp
  EditableTableModel.cpp
  PrependRowTableModel.cpp
  AppendRowTableModel.cpp
  AppendRecordTableModel.cpp
  InsertRowsTableModel.cpp
  RemoveFirstRowTableModel.cpp
  RemoveLastRowTableModel.cpp
  RemoveRowsTableModel.cpp
  DefaultHeaderTableModel.cpp
  CustomHeaderTableModel.cpp
  ItemSelectionModelTester.cpp
)
add_library(Mdt::ItemModelTestCommon ALIAS Mdt_ItemModelTestCommon)

target_include_directories(Mdt_ItemModelTestCommon
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(Mdt_ItemModelTestCommon
  PUBLIC
    Mdt::ItemModel
    Mdt::ItemModelTestLib
    Mdt0::Numeric
)
