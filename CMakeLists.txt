cmake_minimum_required (VERSION 2.6)

project (openxbox)

if(WIN32)
	if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
		set(BUILD_PLATFORM win64)
	else()
		set(BUILD_PLATFORM win32)
	endif()

	set(ENV{PKG_CONFIG_PATH} "${CMAKE_SOURCE_DIR}/extern/glib-2.48.2/${BUILD_PLATFORM}/lib/pkgconfig/;$ENV{PKG_CONFIG_PATH}")
	set(PKG_CONFIG_EXECUTABLE "${CMAKE_SOURCE_DIR}/tools/pkg-config/bin/pkg-config.exe")

	set(SDL2_PATH "${CMAKE_SOURCE_DIR}/extern/SDL2-2.0.7")

	set(GLEW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/extern/glew-2.1.0/${BUILD_PLATFORM}/include")
	set(GLEW_LIBRARY_RELEASE "${CMAKE_SOURCE_DIR}/extern/glew-2.1.0/${BUILD_PLATFORM}/lib/glew32.lib")

	set(LIBUNICORN_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/extern/unicorn-1.0.1/${BUILD_PLATFORM}/include")
	set(LIBUNICORN_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/extern/unicorn-1.0.1/${BUILD_PLATFORM}")

	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

option(USE_CPU_UNICORN "Use Unicorn CPU emulator" ON)

# Add source directories
set(SOURCES, "")
add_subdirectory("${CMAKE_SOURCE_DIR}/src")

# Add Visual Studio filters to better organize the code
if(WIN32)
	foreach(FILE ${SOURCES}) 
	    # Get the directory of the source file
	    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

	    # Remove common directory prefix to make the group
	    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/src" "" GROUP "${PARENT_DIR}")

	    # Make sure we are using windows slashes
	    string(REPLACE "/" "\\" GROUP "${GROUP}")

	    # Group into "Source Files" and "Header Files"
	    if("${FILE}" MATCHES ".*\\.c(pp)?")
	       set(GROUP "Source Files${GROUP}")
	    elseif("${FILE}" MATCHES ".*\\.h")
	       set(GROUP "Header Files${GROUP}")
	    endif()

	    source_group("${GROUP}" FILES "${FILE}")
	endforeach()
endif()

# Main Executable
if(NOT WIN32)
	add_definitions("-Wall -Werror -O0 -g")
endif()
add_executable(openxbox ${SOURCES})
target_include_directories(openxbox PUBLIC ${CMAKE_SOURCE_DIR}/src)

# SDL2
find_package(SDL2 REQUIRED)
target_link_libraries(openxbox ${SDL2_LIBRARY})
target_include_directories(openxbox PUBLIC ${SDL2_INCLUDE_DIR})

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(openxbox ${OPENGL_gl_LIBRARY})
target_include_directories(openxbox PUBLIC ${OPENGL_INCLUDE_DIR})

# GL Extension Wrangler (GLEW)
find_package(GLEW REQUIRED)
target_link_libraries(openxbox ${GLEW_LIBRARIES})
target_include_directories(openxbox PUBLIC ${GLEW_INCLUDE_DIRS})

# Unicorn CPU Emulator
if (USE_CPU_UNICORN)
find_package(Unicorn REQUIRED)
add_definitions("-DCPU_UNICORN")
target_link_libraries(openxbox ${LIBUNICORN_LIBRARY})
target_include_directories(openxbox PUBLIC ${LIBUNICORN_INCLUDE_DIR})
endif (USE_CPU_UNICORN)

# glib
find_package(PkgConfig)
pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_search_module(GOBJ REQUIRED gobject-2.0)
target_link_libraries(openxbox ${GLIB_LDFLAGS} ${GOBJ_LDFLAGS})
target_include_directories(openxbox PUBLIC ${GLIB_INCLUDE_DIRS})

if(WIN32)
	add_custom_command(TARGET openxbox
		POST_BUILD
		COMMAND copy /b /y $(ProjectDir)..\\extern\\glew-2.1.0\\win64\\bin\\glew32.dll $(TargetDir)
		COMMAND copy /b /y $(ProjectDir)..\\extern\\SDL2-2.0.7\\lib\\x64\\SDL2.dll $(TargetDir)
		COMMAND copy /b /y $(ProjectDir)..\\extern\\unicorn-1.0.1\\win64\\unicorn.dll $(TargetDir)
		COMMAND copy /b /y $(ProjectDir)..\\extern\\unicorn-1.0.1\\win64\\libgcc_s_seh-1.dll $(TargetDir)
		COMMAND copy /b /y $(ProjectDir)..\\extern\\unicorn-1.0.1\\win64\\libwinpthread-1.dll $(TargetDir)
		COMMAND copy /b /y $(ProjectDir)..\\extern\\glib-2.48.2\\win64\\bin\\glib-2-vs10.dll $(TargetDir)
		COMMAND copy /b /y $(ProjectDir)..\\extern\\glib-2.48.2\\win64\\bin\\gobject-2-vs10.dll $(TargetDir)
		COMMENT "Copying DLLs to output directory")
endif()