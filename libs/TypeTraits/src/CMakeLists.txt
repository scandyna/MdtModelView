
include(GenerateExportHeader)
include(MdtInstallLibrary)

add_library(Mdt_TypeTraits
  Mdt/TypeTraits/detected_or.cpp
  Mdt/TypeTraits/is_detected.cpp
)

add_library(Mdt::TypeTraits ALIAS Mdt_TypeTraits)

if(BUILD_USE_IPO_LTO)
  set_target_properties(Mdt_TypeTraits
    PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()

if(NOT MSVC)
  target_compile_definitions(Mdt_TypeTraits
    PUBLIC
      MDT_TYPE_TRAITS_COMPILER_SUPPORTS_DETECTION_IDIOM
  )
endif()

# target_compile_definitions(Mdt_TypeTraits
#   PRIVATE
#     QT_NO_CAST_DEFINITIONS QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_BYTEARRAY
# )

target_include_directories(Mdt_TypeTraits
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

# target_link_libraries(Mdt_TypeTraits
#   PUBLIC
#     Qt5::Core
#     Mdt0::Numeric
# )

generate_export_header(Mdt_TypeTraits)

mdt_install_library(
  TARGET Mdt_TypeTraits
  RUNTIME_DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY_DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE_DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES_DIRECTORY .
  INCLUDES_FILE_WITHOUT_EXTENSION
  ADDITIONAL_INCLUDES_FILES "${CMAKE_CURRENT_BINARY_DIR}/mdt_typetraits_export.h"
  INCLUDES_DESTINATION ${MDT_INSTALL_INCLUDEDIR}
  EXPORT_NAME TypeTraits
  EXPORT_NAMESPACE Mdt${PROJECT_VERSION_MAJOR}::
  INSTALL_NAMESPACE ${MDT_INSTALL_PACKAGE_NAME}
  FIND_PACKAGE_PATHS ..
  INSTALL_IS_UNIX_SYSTEM_WIDE ${MDT_INSTALL_IS_UNIX_SYSTEM_WIDE}
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  VERSION_COMPATIBILITY ExactVersion
  RUNTIME_COMPONENT Mdt_TypeTraits_Runtime
  DEVELOPMENT_COMPONENT Mdt_TypeTraits_Dev
)
