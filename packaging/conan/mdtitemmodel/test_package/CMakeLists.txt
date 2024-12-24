cmake_minimum_required(VERSION 3.15)
project(MdtItemModelPackageTest LANGUAGES CXX)

find_package(MdtCMakeModules REQUIRED)
find_package(Mdt0 REQUIRED COMPONENTS ItemModel)

set(CMAKE_AUTOMOC ON)

include(MdtAddTest)

enable_testing()

mdt_add_test(
  NAME ItemModelPackageTest
  TARGET itemModelPackageTest
  DEPENDENCIES Mdt0::ItemModel
  SOURCE_FILES
    main.cpp
)
