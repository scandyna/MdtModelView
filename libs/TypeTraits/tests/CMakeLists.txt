
include(MdtAddTest)


# TODO: check if Catch2Main is required
mdt_add_test(
  NAME IsDetectedTest
  TARGET isDetectedTest
  DEPENDENCIES Mdt::TypeTraits Mdt::Catch2Main
  SOURCE_FILES
    src/IsDetectedTest.cpp
)
