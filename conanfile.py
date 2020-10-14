from conans import ConanFile, CMake, tools


class OverpeekEngineConan(ConanFile):
    name = "overpeek-engine"
    version = "0.1"
    license = "MIT"
    author = "Eemeli Lehtonen"
    url = "https://github.com/Overpeek/overpeek-engine"
    description = "A minimal 2D game engine/library."
    topics = ("game-engine", "cpp17", "opengl")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    generators = "cmake"
    requires = "openal/1.20.1", "glfw/3.3.2", "glad/0.1.33", "box2d/2.4.0", "libzip/1.7.3", "enet/1.3.16", "freetype/2.10.2", "ms-gsl/3.1.0", "entt/3.5.2", "fmt/7.0.3", "spdlog/1.8.0", "stb/20200203", "minimp3/20200304", "glm/0.9.9.5", "gcem/1.12.0"
    default_options = {"shared": False, "glad:gl_version": "4.6", "fmt:header_only": True, "spdlog:header_only": True,
                       "openal:shared": True, "glfw:shared": True, "box2d:shared": True, "libzip:shared": True, "zlib:shared": True, "freetype:shared": True}
    keep_imports = True

    def source(self):
        # This small hack might be useful to guarantee proper /MT /MD linkage
        # in MSVC if the packaged project doesn't have variables to set it
        # properly
        tools.replace_in_file("overpeek-engine/CMakeLists.txt", 'project("engine")',
                              '''project("engine")
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()''')

    def build(self):
        cmake = CMake(self)
        cmake.verbose = True
        cmake.configure(source_folder=".", build_folder="build")
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def imports(self):
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dll", dst="tests", keep_path=False)

    def package(self):
        self.copy("*.hpp", dst="include", src="src/engine")
        self.copy("*.exe", dst="bin", keep_path=False)
        self.copy("entities", dst="bin", keep_path=False)
        self.copy("guis", dst="bin", keep_path=False)
        self.copy("networking", dst="bin", keep_path=False)
        self.copy("rendering", dst="bin", keep_path=False)
        self.copy("text", dst="bin", keep_path=False)
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["engine"]
