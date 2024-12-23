# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

add_executable(itemselectionmodeltestwidget
  TableModel.cpp
  ItemSelectionModelTestWidget.cpp
  main.cpp
)

target_link_libraries(itemselectionmodeltestwidget
  PRIVATE
    Mdt::ItemModel
    Mdt::ItemView_QtWidgets
    Qt5::Widgets
)

if(BUILD_USE_IPO_LTO)
  set_target_properties(itemselectionmodeltestwidget
    PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()

target_include_directories(itemselectionmodeltestwidget
  PRIVATE
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
