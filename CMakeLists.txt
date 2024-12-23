cmake_minimum_required(VERSION 3.15)

# CMAKE_PREFIX_PATH is empty before any call to project()
# Because set_project_version() does not exist,
# we call the project command twice (is there a cleaner solution?)
project(MdtModelView)

#######################################################
# Project definition with versionning got from git tag
#######################################################

find_package(Git REQUIRED)
find_package(MdtCMakeModules REQUIRED)

include(MdtVersionUtils)

set(INITIAL_PROJECT_VERSION)
if(FROM_CONAN_PROJECT_VERSION)
  set(INITIAL_PROJECT_VERSION ${FROM_CONAN_PROJECT_VERSION})
else()
  mdt_cmake_project_version_from_git_tag(INITIAL_PROJECT_VERSION DEFAULT_VERSION 0.0.0)
endif()

project(MdtModelView VERSION ${INITIAL_PROJECT_VERSION} LANGUAGES CXX)

unset(INITIAL_PROJECT_VERSION)

#######################
# Options
#######################

include(MdtBuildOptionsUtils)
include(MdtSanitizers)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

# option(BUILD_APPS "Build the applications (tools)" OFF)
option(BUILD_TESTS "Build the tests" OFF)
option(BUILD_BENCHMARKS "Build the benchmarks" OFF)
option(BUILD_DOCS "Build the documentations" OFF)
mdt_set_available_build_types(Release Debug RelWithDebInfo MinSizeRel)
option(WARNING_AS_ERROR "Treat warnings as errors" OFF)

option(BUILD_USE_IPO_LTO_IF_AVAILABLE "Use link-time optimization if available on the platform" OFF)
mark_as_advanced(BUILD_USE_IPO_LTO_IF_AVAILABLE)
# Provide LPO/LTO option if supported
# Note: CMake before 3.9 does only support Intel compiler on Linux.
#       Check documentation of the CheckIPOSupported module,
#       and also CMP0069 policy.
include(CheckIPOSupported)
check_ipo_supported(RESULT HAVE_IPO_LTO)
if(HAVE_IPO_LTO)
  option(BUILD_USE_IPO_LTO "Use link-time optimization" ${BUILD_USE_IPO_LTO_IF_AVAILABLE})
endif()

mdt_add_address_sanitizer_option_if_available(SANITIZER_ENABLE_ADDRESS
  HELP_STRING "Enable address sanitizer for RelWithDebInfo, Debug build"
  INITIAL_VALUE OFF
)
mdt_add_memory_sanitizer_option_if_available(SANITIZER_ENABLE_MEMORY
  HELP_STRING "Enable memory sanitizer for RelWithDebInfo, Debug build"
  INITIAL_VALUE OFF
)
mdt_add_undefined_sanitizer_option_if_available(SANITIZER_ENABLE_UNDEFINED
  HELP_STRING "Enable undefined behaviour sanitizer for RelWithDebInfo, Debug build"
  INITIAL_VALUE OFF
)
mdt_add_thread_sanitizer_option_if_available(SANITIZER_ENABLE_THREAD
  HELP_STRING "Enable thread sanitizer for RelWithDebInfo, Debug build (can be incompatible with other sanitizers)"
  INITIAL_VALUE OFF
)

#######################
# Warnings
#######################

if(MSVC)
  add_compile_options(/W3)
  if(WARNING_AS_ERROR)
    add_compile_options(/WX)
  endif()
  # Disable some Warnigs on MSVC:
  #  C4251: 'type' : class 'type1' needs to have dll-interface to be used by clients of class 'type2'
  #         Probably a warning on binary compatibility, which we don't support anyway
  #         See also: https://stackoverflow.com/questions/32098001/stdunique-ptr-pimpl-in-dll-generates-c4251-with-visual-studio
  add_compile_options(/wd4251)
else()
  # TODO: check about -Wshadow
  add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wunused)
  if(WARNING_AS_ERROR)
    add_compile_options(-Werror)
  endif()
endif()

#######################
# Compilers issues
#######################

# Workaround for the "too many sections" error on some MinGW compiler
# See: https://stackoverflow.com/questions/16596876/object-file-has-too-many-sections
if(MINGW)
  add_compile_options(-Wa,-mbig-obj)
endif()

#######################
# Dependencies
#######################

find_package(Threads REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core)

# if(BUILD_APPS)
#   find_package(Qt5 REQUIRED COMPONENTS Widgets)
# endif()

# For header only library,no component is defined.
# https://cmake.org/pipermail/cmake/2013-September/055941.html
# find_package(Boost REQUIRED)

if(BUILD_TESTS)
  find_package(Catch2 REQUIRED)
#   find_package(Qt5 REQUIRED COMPONENTS Test)
endif()

if(BUILD_BENCHMARKS)
#   find_package(Catch2 REQUIRED)
  find_package(Qt5 REQUIRED COMPONENTS Test)
endif()

if(BUILD_DOCS)
  include(FindDoxygen)
  find_package(Doxygen REQUIRED dot)
endif()

#######################
# Sanitizers
#######################

if(SANITIZER_ENABLE_ADDRESS)
  mdt_build_with_address_sanitizer(BUILD_TYPES RelWithDebInfo Debug)
endif()
if(SANITIZER_ENABLE_MEMORY)
  mdt_build_with_memory_sanitizer(BUILD_TYPES RelWithDebInfo Debug)
endif()
if(SANITIZER_ENABLE_UNDEFINED)
  mdt_build_with_undefined_sanitizer(BUILD_TYPES RelWithDebInfo Debug)
endif()
if(SANITIZER_ENABLE_THREAD)
  mdt_build_with_thread_sanitizer(BUILD_TYPES RelWithDebInfo Debug)
endif()

#######################
# Install
#######################

set(MDT_INSTALL_PACKAGE_NAME Mdt${PROJECT_VERSION_MAJOR})
include(GNUInstallDirs)
include(MdtInstallDirs)
include(MdtPackageConfigHelpers)

#######################
# Qt
#######################

# TODO: remove once migration to Qt6 done
# See https://doc.qt.io/qt-6/portingguide.html
add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x050F00)

# TODO: remove before migrate to Qt6
# add_compile_definitions(QT_NO_DEPRECATED_WARNINGS=1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

###############################
# Binary outputs in build tree
###############################

# On Windows some tests fails because of missing dll's
# See https://gitlab.com/scandyna/mdt-cmake-modules/-/issues/4
if(WIN32)
  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/BuildOutputs/bin")
  endif()
  if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/BuildOutputs/lib")
  endif()
  if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/BuildOutputs/lib")
  endif()
endif()

#######################
# Sources
#######################

if(BUILD_TESTS OR BUILD_BENCHMARKS)
  enable_testing()
endif()

add_subdirectory(libs)

# if(BUILD_APPS)
#   add_subdirectory(apps)
# endif()

if(BUILD_DOCS)
  add_subdirectory(doc)
endif()
