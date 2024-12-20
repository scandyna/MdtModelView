from conan import ConanFile
from conan.tools.env import VirtualBuildEnv
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake

# This recipe is only to install dependencies to build MdtModelView
# The recipes to create packages are in packaging/conan/ subfolder
class MdtModelViewConan(ConanFile):
  name = "mdtmodelview"
  license = "LGPL-3.0-or-later"
  url = "https://gitlab.com/scandyna/mdtmodelview"
  description = "Set of libraries extending the Qt model-view framework."
  settings = "os", "compiler", "build_type", "arch"
  options = {"shared": [True, False]}
  default_options = {"shared": True}
  generators = "CMakeDeps", "VirtualBuildEnv"

  def requirements(self):
    self.requires("qt/5.15.6")

  def build_requirements(self):
    self.test_requires("catch2/2.13.9")
    self.test_requires("mdtcmakemodules/0.20.0@scandyna/testing")

  def generate(self):
    tc = CMakeToolchain(self)
    #tc.variables["FROM_CONAN_PROJECT_VERSION"] = self.version
    tc.generate()
