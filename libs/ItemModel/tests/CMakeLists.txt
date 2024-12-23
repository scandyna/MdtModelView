# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

include(MdtAddTest)

add_subdirectory(src/common)


mdt_add_test(
  NAME NumericLimitsTest
  TARGET numericLimitsTest
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestLib Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/NumericLimitsTest.cpp
)

mdt_add_test(
  NAME AbstractTableModel_ReadOnly_Test
  TARGET abstractTableModel_ReadOnly_Test
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestLib Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/AbstractTableModel_ReadOnly_Test.cpp
)

mdt_add_test(
  NAME AbstractTableModel_CustomRoles_Test
  TARGET abstractTableModel_CustomRoles_Test
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestLib Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/AbstractTableModel_CustomRoles_Test.cpp
)

mdt_add_test(
  NAME AbstractTableModel_Editable_Test
  TARGET abstractTableModel_Editable_Test
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestLib Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/AbstractTableModel_Editable_Test.cpp
)

# Some tests that depends on Qt TestLib,
# like QAbstractItemModelTester
# (available in the public API since Qt 5.11)
mdt_add_test(
  NAME AbstractTableModelQTLTest
  TARGET abstractTableModelQTLTest
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestCommon Qt5::Test
  SOURCE_FILES
    src/AbstractTableModelQTLTest.cpp
)

mdt_add_test(
  NAME AbstractTableModel_InsertRows_Test
  TARGET abstractTableModel_InsertRows_Test
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestLib Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/AbstractTableModel_InsertRows_Test.cpp
)

mdt_add_test(
  NAME AbstractTableModel_PrependRow_Test
  TARGET abstractTableModel_PrependRow_Test
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestLib Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/AbstractTableModel_PrependRow_Test.cpp
)

mdt_add_test(
  NAME AbstractTableModel_AppendRow_Test
  TARGET abstractTableModel_AppendRow_Test
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestLib Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/AbstractTableModel_AppendRow_Test.cpp
)

mdt_add_test(
  NAME AbstractTableModel_AppendRecord_Test
  TARGET abstractTableModel_AppendRecord_Test
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestLib Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/AbstractTableModel_AppendRecord_Test.cpp
)

mdt_add_test(
  NAME AbstractTableModel_RemoveRows_Test
  TARGET abstractTableModel_RemoveRows_Test
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestLib Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/AbstractTableModel_RemoveRows_Test.cpp
)

mdt_add_test(
  NAME AbstractTableModel_RemoveFirstRow_Test
  TARGET abstractTableModel_RemoveFirstRow_Test
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestLib Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/AbstractTableModel_RemoveFirstRow_Test.cpp
)

mdt_add_test(
  NAME AbstractTableModel_RemoveLastRow_Test
  TARGET abstractTableModel_RemoveLastRow_Test
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestLib Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/AbstractTableModel_RemoveLastRow_Test.cpp
)

mdt_add_test(
  NAME ProxyModelPipelineTest
  TARGET proxyModelPipelineTest
  DEPENDENCIES Mdt::ItemModel Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ProxyModelPipelineTest.cpp
)

mdt_add_test(
  NAME RowRangeTest
  TARGET rowRangeTest
  DEPENDENCIES Mdt::ItemModel Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/RowRangeTest.cpp
)

mdt_add_test(
  NAME RowRangeListAlgorithmTest
  TARGET rowRangeListAlgorithmTest
  DEPENDENCIES Mdt::ItemModel Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/RowRangeListAlgorithmTest.cpp
)

mdt_add_test(
  NAME RowRangeListTest
  TARGET rowRangeListTest
  DEPENDENCIES Mdt::ItemModel Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/RowRangeListTest.cpp
)

mdt_add_test(
  NAME RowSelectionHelpersTest
  TARGET rowSelectionHelpersTest
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/RowSelectionHelpersTest.cpp
)

mdt_add_test(
  NAME RowSelectionTest
  TARGET rowSelectionTest
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/RowSelectionTest.cpp
)

mdt_add_test(
  NAME RowListViewConstIteratorTest
  TARGET rowListViewConstIteratorTest
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/RowListViewConstIteratorTest.cpp
)

mdt_add_test(
  NAME RowListViewTest
  TARGET rowListViewTest
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/RowListViewTest.cpp
)

mdt_add_test(
  NAME ReverseRowListViewConstIteratorTest
  TARGET reverseRowListViewConstIteratorTest
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ReverseRowListViewConstIteratorTest.cpp
)

mdt_add_test(
  NAME ReverseRowListViewTest
  TARGET reverseRowListViewTest
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ReverseRowListViewTest.cpp
)

mdt_add_test(
  NAME ItemSelectionModelTest
  TARGET itemSelectionModelTest
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ItemSelectionModelTest.cpp
)

mdt_add_test(
  NAME ItemSelectionModelWidgetsTest
  TARGET itemSelectionModelWidgetsTest
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt Qt5::Test Qt5::Widgets
  SOURCE_FILES
    src/ItemSelectionModelWidgetsTest.cpp
)

mdt_add_test(
  NAME ItemModelHelpersTest
  TARGET itemModelHelpersTest
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ItemModelHelpersTest.cpp
)

mdt_add_test(
  NAME ItemModelStlHelpersTest
  TARGET itemModelStlHelpersTest
  DEPENDENCIES Mdt::ItemModel Mdt::ItemModelTestCommon Mdt::Catch2Main Mdt::Catch2Qt
  SOURCE_FILES
    src/ItemModelStlHelpersTest.cpp
)
