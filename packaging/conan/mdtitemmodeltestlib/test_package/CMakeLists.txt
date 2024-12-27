cmake_minimum_required(VERSION 3.15)
project(MdtItemModelTestLibPackageTest LANGUAGES CXX)

find_package(MdtCMakeModules REQUIRED)
find_package(Mdt0 REQUIRED COMPONENTS ItemModelTestLib)

set(CMAKE_AUTOMOC ON)

include(MdtAddTest)

enable_testing()

mdt_add_test(
  NAME ItemModelTestLibPackageTest
  TARGET itemModelTestLibPackageTest
  DEPENDENCIES Mdt0::ItemModelTestLib
  SOURCE_FILES
    main.cpp
)
