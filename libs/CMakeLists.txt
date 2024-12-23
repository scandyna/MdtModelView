# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

add_subdirectory(ItemModel)
add_subdirectory(ItemModel_TestLib)
add_subdirectory(ItemView_QtWidgets)

# TODO: TestLib should only be added if tests/benchamrks are built ?
# add_subdirectory(TestLib)

if(BUILD_TESTS OR BUILD_BENCHMARKS)
  add_subdirectory(Catch2)
endif()
