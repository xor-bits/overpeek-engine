cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(source_list "")
set(source_list ${source_list}
	"src/include/nfd.h"
	"src/common.h"
	"src/nfd_common.h"
	"src/simple_exec.h"
	"src/nfd_common.c"
)
if (MSVC)
	set(source_list ${source_list}
		"src/nfd_win.cpp"
	)
elseif(EMSCRIPTEN)
	# idk
elseif(APPLE)
	set(source_list ${source_list}
		"src/nfd_cocoa.m"
	)
elseif(UNIX)
	set(source_list ${source_list}
		# GTK or zenity
		"src/nfd_zenity.c"
		# "src/nfd_gtk.c"
	)
endif()

set(target_name "nativefiledialog")
add_library(${target_name} STATIC ${source_list})
set_target_properties(${target_name} PROPERTIES FOLDER engine)