# Distributed under the OSI-approved BSD 3-Clause License.
# See accompanying file Copyright.txt or https://cmake.org/licensing for details.

include(MdtAddTest)


mdt_add_test(
  NAME DataChangedSignalSpyTest
  TARGET dataChangedSignalSpyTest
  DEPENDENCIES Mdt::ItemModelTestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/DataChangedSignalSpyTest.cpp
)

mdt_add_test(
  NAME InsertRowsSignalsSpyTest
  TARGET insertRowsSignalsSpyTest
  DEPENDENCIES Mdt::ItemModelTestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/InsertRowsSignalsSpyTest.cpp
)

mdt_add_test(
  NAME RemoveRowsSignalsSpyTest
  TARGET removeRowsSignalsSpyTest
  DEPENDENCIES Mdt::ItemModelTestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/RemoveRowsSignalsSpyTest.cpp
)

mdt_add_test(
  NAME TestLibRowSelectionHelpersTest
  TARGET testLibRowSelectionHelpersTest
  DEPENDENCIES Mdt::ItemModelTestLib Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/TestLibRowSelectionHelpersTest.cpp
)
