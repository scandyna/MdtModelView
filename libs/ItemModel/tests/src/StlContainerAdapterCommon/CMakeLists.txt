
add_library(Mdt_ItemModelStlContainerAdapterTestCommon STATIC
  Item.cpp
  ReadOnlyList.cpp
  DefaultConstructibleOnlyList.cpp
  CopyConstructibleOnlyList.cpp
  MoveConstructibleOnlyList.cpp
  ReadOnlyListTableModelAdapterFunctionMap.cpp
  DefaultConstructibleOnlyListTableModelAdapterFunctionMap.cpp
  CopyConstructibleOnlyListTableModelAdapterFunctionMap.cpp
  MoveConstructibleOnlyListTableModelAdapterFunctionMap.cpp
  MutableListRawData.cpp
  MutableListRawDataTableModelAdapterFunctionMap.cpp
  MutableList.cpp
  MutableListTableModelAdapterFunctionMap.cpp
  ReadOnlyResizableList.cpp
  ReadOnlyResizableListTableModelAdapterFunctionMap.cpp
)
add_library(Mdt::ItemModelStlContainerAdapterTestCommon ALIAS Mdt_ItemModelStlContainerAdapterTestCommon)

target_include_directories(Mdt_ItemModelStlContainerAdapterTestCommon
  PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(Mdt_ItemModelStlContainerAdapterTestCommon
  PUBLIC
    Mdt::ItemModel
)
