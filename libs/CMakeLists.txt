# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

if(BUILD_ITEM_MODEL)
  add_subdirectory(ItemModel)
endif()

if(BUILD_ITEM_MODEL_TEST_LIB)
  add_subdirectory(ItemModel_TestLib)
endif()

if(BUILD_ITEM_VIEW_QT_WIDGETS)
  add_subdirectory(ItemView_QtWidgets)
endif()

# TODO: TestLib should only be added if tests/benchamrks are built ?
# add_subdirectory(TestLib)

if(BUILD_TESTS OR BUILD_BENCHMARKS)
  add_subdirectory(Catch2)
endif()
