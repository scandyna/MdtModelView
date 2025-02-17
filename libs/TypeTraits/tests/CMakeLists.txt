
include(MdtAddTest)


mdt_add_test(
  NAME DetectedOrTest
  TARGET detectedOrTest
  DEPENDENCIES Mdt::TypeTraits
  SOURCE_FILES
    src/DetectedOrTest.cpp
)

mdt_add_test(
  NAME IsDetectedTest
  TARGET isDetectedTest
  DEPENDENCIES Mdt::TypeTraits Mdt::Catch2Main
  SOURCE_FILES
    src/IsDetectedTest.cpp
)

mdt_add_test(
  NAME StlContainerHelpersTest
  TARGET stlContainerHelpersTest
  DEPENDENCIES Mdt::TypeTraits Mdt::Catch2Main
  SOURCE_FILES
    src/StlContainerHelpersTest.cpp
)
