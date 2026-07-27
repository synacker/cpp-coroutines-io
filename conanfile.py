from conan import ConanFile
from conan.tools.cmake import cmake_layout

class IOUringStdexecServer(ConanFile):
    name = "uring_stdexec_server"
    version = "0.1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("liburing/2.13")
        self.requires("p2300/nvhpc-26.05") # Grabs the latest reference release

    def layout(self):
        cmake_layout(self)
