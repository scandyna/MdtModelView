from conan import ConanFile
from conan.tools.env import VirtualBuildEnv
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout
from conan.tools.files import copy
import os

class MdtItemModelTestLibConan(ConanFile):
  name = "mdtitemmodeltestlib"
  license = "LGPL-3.0-or-later"
  url = "https://gitlab.com/scandyna/mdtmodelview"
  description = "Set of libraries extending the Qt model-view framework."
  settings = "os", "compiler", "build_type", "arch"
  options = {"shared": [True, False]}
  default_options = {"shared": True}
  generators = "CMakeDeps", "VirtualBuildEnv"

  # See: https://docs.conan.io/en/latest/reference/conanfile/attributes.html#short-paths
  #short_paths = True

  def set_version(self):
    if not self.version:
      self.version = "0.0.0"

  def requirements(self):
    self.requires("qt/5.15.6")
    self.requires("mdtcmakeconfig/0.1.0@scandyna/testing")
    self.requires("mdtnumeric/0.0.1@scandyna/testing")
    self.requires(f"mdtitemmodel/{self.version}@scandyna/testing")

  def build_requirements(self):
    self.test_requires("mdtcmakemodules/0.20.0@scandyna/testing")

  def export_sources(self):
    source_root = os.path.join(self.recipe_folder, "../../../")
    copy(self, "CMakeLists.txt", source_root, self.export_sources_folder)
    copy(self, "COPYING", source_root, self.export_sources_folder)
    copy(self, "COPYING.LESSER", source_root, self.export_sources_folder)
    copy(self, "README.md", source_root, self.export_sources_folder)
    copy(self, "libs/CMakeLists.txt", source_root, self.export_sources_folder)
    copy(self, "libs/ItemModel_TestLib/*", source_root, self.export_sources_folder)

  def layout(self):
    cmake_layout(self)

  def generate(self):
    tc = CMakeToolchain(self)
    tc.variables["FROM_CONAN_PROJECT_VERSION"] = self.version
    tc.variables["BUILD_ITEM_MODEL"] = "OFF"
    tc.variables["USE_ITEM_MODEL_IMPORT_TARGET"] = "ON"
    tc.variables["BUILD_ITEM_VIEW_QT_WIDGETS"] = "OFF"
    tc.variables["BUILD_ITEM_MODEL_TEST_LIB"] = "ON"
    tc.generate()

  def build(self):
    cmake = CMake(self)
    cmake.configure()
    cmake.build()

  def package(self):
    cmake = CMake(self)
    cmake.install()

  # Use the default package_id()
  # https://docs.conan.io/en/latest/creating_packages/define_abi_compatibility.html#define-abi-compatibility

  def package_info(self):
    self.cpp_info.set_property("cmake_file_name", "Mdt0ItemModelTestLib")
    self.cpp_info.set_property("cmake_target_name", "Mdt0::ItemModelTestLib")
    # We have to specify the components of Qt to use, otherwise we will depend on all
    # See also https://gitlab.com/scandyna/mdtmodelview/-/issues/2
    self.cpp_info.requires = ["mdtitemmodel::mdtitemmodel", "mdtnumeric::mdtnumeric", "qt::qtCore"]
    self.cpp_info.libs = ["Mdt0ItemModelTestLib"]
