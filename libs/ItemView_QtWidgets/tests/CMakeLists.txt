# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

include(MdtAddTest)

add_subdirectory(src/common)


mdt_add_test(
  NAME ItemViewQtWidgetsHelpersTest
  TARGET itemViewQtWidgetsHelpersTest
  DEPENDENCIES Mdt::ItemView_QtWidgets Mdt::ItemViewQtWidgetsTestCommon Mdt::ItemModelTestLib Qt5::Test
  SOURCE_FILES
    src/ItemViewHelpersTest.cpp
)

# TODO: remove once test fixed
# See https://gitlab.com/scandyna/mdtmodelview/-/issues/3
if(SANITIZER_ENABLE_ADDRESS OR SANITIZER_ENABLE_UNDEFINED)
  set_tests_properties(ItemViewQtWidgetsHelpersTest
    PROPERTIES
      DISABLED True
  )
endif()
