from conans import ConanFile, CMake, tools
import os


class OverpeekEngineConan(ConanFile):
	name = "overpeek-engine"
	version = "0.1"
	license = "MIT"
	author = "Eemeli Lehtonen"
	url = "https://github.com/Overpeek/overpeek-engine"
	description = "A minimal 2D game engine/library."
	topics = ("game-engine", "cpp17", "opengl")
	settings = "os", "compiler", "build_type", "arch"
	options = {"shared": [True, False], "oe_build_tests": [True, False], "oe_build_mode": ["opengl", "shaderc", "vulkan"]}
	generators = "cmake"
	requires = "glfw/3.3.2", "glad/0.1.33", "box2d/2.4.0", "libzip/1.7.3", "enet/1.3.16", "ms-gsl/3.1.0", "entt/3.5.2", "fmt/7.0.3", "spdlog/1.8.0", "stb/20200203", "minimp3/20200304", "glm/0.9.9.5", "gcem/1.12.0"
	default_options = {"shared": False, "oe_build_tests": True, "oe_build_mode": "opengl", "glad:gl_version": "4.6", "fmt:header_only": True, "spdlog:header_only": True}
	keep_imports = True
	exports_sources = "*"

	def requirements(self):
		# Or add a new requirement!
		if self.options.oe_build_mode == "shaderc" or self.options.oe_build_mode == "vulkan":
			self.requires("shaderc/2019.0")
		if self.options.oe_build_mode == "vulkan":
			self.requires("vulkan-headers/1.1.101.0")

	def source(self):
		# This small hack might be useful to guarantee proper /MT /MD linkage
		# in MSVC if the packaged project doesn't have variables to set it
		# properly
		tools.replace_in_file("CMakeLists.txt",
			'include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)',
			'include(conanbuildinfo.cmake)')

	def build(self):
		cmake = CMake(self)
		defs = { "OE_BUILD_MODE": 0, "OE_BUILD_TESTS": True }
		if self.options.oe_build_mode == "vulkan":
			defs["OE_BUILD_MODE"] = 2
		elif self.options.oe_build_mode == "shaderc":
			defs["OE_BUILD_MODE"] = 1
		defs["OE_BUILD_TESTS"] = self.options.oe_build_tests
		cmake.configure(defs=defs)
		cmake.build()
		if tools.get_env("OE_RUN_TESTS", False):
			self.run("cd tests && ctest -j{} --output-on-failure".format(tools.cpu_count()))

	def imports(self):
		""" self.copy("*.dylib", dst="lib", keep_path=False)
		self.copy("*.so", dst="lib", keep_path=False)
		self.copy("*.dll", dst="bin", keep_path=False)
		self.copy("*.dll", dst="tests", keep_path=False)"""
		self.copy("*/license*", dst="licenses", folder=True, ignore_case=True)

	def package(self):
		self.copy("*.hpp", dst="include/engine", src="src/engine")
		self.copy("bin/*", dst="bin", keep_path=False)
		self.copy("*.lib", dst="lib", keep_path=False)
		self.copy("*.a", dst="lib", keep_path=False)

	def package_info(self):
		self.cpp_info.libs = ["engine"]
