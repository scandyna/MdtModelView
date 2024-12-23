# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

include(GenerateExportHeader)
include(MdtInstallLibrary)

add_library(Mdt_ItemView_QtWidgets
  Mdt/ItemView/Helpers.cpp
)

add_library(Mdt::ItemView_QtWidgets ALIAS Mdt_ItemView_QtWidgets)

if(BUILD_USE_IPO_LTO)
  set_target_properties(Mdt_ItemView_QtWidgets
    PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()

target_compile_definitions(Mdt_ItemView_QtWidgets
  PRIVATE
    QT_NO_CAST_DEFINITIONS QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_BYTEARRAY
)

target_include_directories(Mdt_ItemView_QtWidgets
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(Mdt_ItemView_QtWidgets
  PUBLIC
    Mdt::ItemModel
    Qt5::Widgets
)

generate_export_header(Mdt_ItemView_QtWidgets)

mdt_install_library(
  TARGET Mdt_ItemView_QtWidgets
  RUNTIME_DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY_DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE_DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES_DIRECTORY .
  INCLUDES_FILE_WITHOUT_EXTENSION
  ADDITIONAL_INCLUDES_FILES "${CMAKE_CURRENT_BINARY_DIR}/mdt_itemview_qtwidgets_export.h"
  INCLUDES_DESTINATION ${MDT_INSTALL_INCLUDEDIR}
  EXPORT_NAME ItemViewQtWidgets
  EXPORT_NAMESPACE Mdt${PROJECT_VERSION_MAJOR}::
  INSTALL_NAMESPACE ${MDT_INSTALL_PACKAGE_NAME}
  FIND_PACKAGE_PATHS ..
  INSTALL_IS_UNIX_SYSTEM_WIDE ${MDT_INSTALL_IS_UNIX_SYSTEM_WIDE}
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  VERSION_COMPATIBILITY ExactVersion
  RUNTIME_COMPONENT Mdt_ItemView_QtWidgets_Runtime
  DEVELOPMENT_COMPONENT Mdt_ItemView_QtWidgets_Dev
)
