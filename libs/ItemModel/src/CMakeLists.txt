# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

include(GenerateExportHeader)
include(MdtInstallLibrary)

add_library(Mdt_ItemModel
  Mdt/ItemModel/NumericLimits.cpp
  Mdt/ItemModel/AbstractTableModel.cpp
  Mdt/ItemModel/ProxyModelPipeline.cpp
  Mdt/ItemModel/RowRange.cpp
  Mdt/ItemModel/RowRangeListAlgorithm.cpp
  Mdt/ItemModel/RowRangeList.cpp
  Mdt/ItemModel/RowSelectionHelpers.cpp
  Mdt/ItemModel/RowSelection.cpp
  Mdt/ItemModel/RowListViewConstIterator.cpp
  Mdt/ItemModel/RowListView.cpp
  Mdt/ItemModel/ReverseRowListViewConstIterator.cpp
  Mdt/ItemModel/ReverseRowListView.cpp
  Mdt/ItemModel/ItemSelectionModel.cpp
  Mdt/ItemModel/Helpers.cpp
  Mdt/ItemModel/StlHelpers.cpp
)

add_library(Mdt::ItemModel ALIAS Mdt_ItemModel)

if(BUILD_USE_IPO_LTO)
  set_target_properties(Mdt_ItemModel
    PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()

target_compile_definitions(Mdt_ItemModel
  PRIVATE
    QT_NO_CAST_DEFINITIONS QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_BYTEARRAY
)

target_include_directories(Mdt_ItemModel
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(Mdt_ItemModel
  PUBLIC
    Qt5::Core
)

generate_export_header(Mdt_ItemModel)

mdt_install_library(
  TARGET Mdt_ItemModel
  RUNTIME_DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY_DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE_DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES_DIRECTORY .
  INCLUDES_FILE_WITHOUT_EXTENSION
  ADDITIONAL_INCLUDES_FILES "${CMAKE_CURRENT_BINARY_DIR}/mdt_itemmodel_export.h"
  INCLUDES_DESTINATION ${MDT_INSTALL_INCLUDEDIR}
  EXPORT_NAME ItemModel
  EXPORT_NAMESPACE Mdt${PROJECT_VERSION_MAJOR}::
  INSTALL_NAMESPACE ${MDT_INSTALL_PACKAGE_NAME}
  FIND_PACKAGE_PATHS ..
  INSTALL_IS_UNIX_SYSTEM_WIDE ${MDT_INSTALL_IS_UNIX_SYSTEM_WIDE}
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  VERSION_COMPATIBILITY ExactVersion
  RUNTIME_COMPONENT Mdt_ItemModel_Runtime
  DEVELOPMENT_COMPONENT Mdt_ItemModel_Dev
)
