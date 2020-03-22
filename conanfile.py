from conans import ConanFile, CMake, tools

class NtgraphConan(ConanFile):
    name = "NTGraph"
    version = "0.1"
    license = "MIT"
    author = "Przemyslaw Koziol przemkoz85@gmail.com"
    url = "https://github.com/selassje/NTGraph"
    description = "2D Graph ActiveX Control"
    topics = ("MFC", "ActiveX", "2D", "Graph")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True]}
    default_options = {"shared": True}
    generators = "cmake"
    exports_sources = "CMakeLists.txt","src/*"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

    def package(self):
        self.copy("*.ocx", dst="bin", keep_path=False)
        self.run("regsvr32 /s " + self.package_folder + "/bin/NTGraph.ocx")

