cmake_minimum_required(VERSION 3.15)
project(MdtItemViewQtWidgetsPackageTest LANGUAGES CXX)

find_package(MdtCMakeModules REQUIRED)
find_package(Mdt0 REQUIRED COMPONENTS ItemViewQtWidgets)

set(CMAKE_AUTOMOC ON)

include(MdtAddTest)

enable_testing()

mdt_add_test(
  NAME ItemViewQtWidgetsPackageTest
  TARGET itemViewQtWidgetsPackageTest
  DEPENDENCIES Mdt0::ItemViewQtWidgets
  SOURCE_FILES
    main.cpp
)
