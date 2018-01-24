if(MSVC)
    cmake_minimum_required (VERSION 3.8)
else()
    cmake_minimum_required (VERSION 2.6)
endif()

project (openxbox)

if(MSVC)
	if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
		set(BUILD_PLATFORM win64)
		set(BUILD_PLATFORM_ALT x64)
	else()
		set(BUILD_PLATFORM win32)
		set(BUILD_PLATFORM_ALT x86)
	endif()

	set(ENV{PKG_CONFIG_PATH} "${CMAKE_SOURCE_DIR}/extern/glib-2.48.2/${BUILD_PLATFORM}/lib/pkgconfig/;$ENV{PKG_CONFIG_PATH}")
	set(PKG_CONFIG_EXECUTABLE "${CMAKE_SOURCE_DIR}/tools/pkg-config/bin/pkg-config.exe")

	set(SDL2_PATH "${CMAKE_SOURCE_DIR}/extern/SDL2-2.0.7")

	set(GLEW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/extern/glew-2.1.0/${BUILD_PLATFORM}/include")
	set(GLEW_LIBRARY_RELEASE "${CMAKE_SOURCE_DIR}/extern/glew-2.1.0/${BUILD_PLATFORM}/lib/glew32.lib")

	set(LIBUNICORN_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/extern/unicorn-1.0.1/${BUILD_PLATFORM}/include")
	set(LIBUNICORN_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/extern/unicorn-1.0.1/${BUILD_PLATFORM}")

	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT openxbox)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

option(USE_CPU_UNICORN "Use Unicorn CPU emulator" ON)

# Add source directories
set(SOURCES, "")
add_subdirectory("${CMAKE_SOURCE_DIR}/src")

# Add Visual Studio filters to better organize the code
if(MSVC)
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
if(NOT MSVC)
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

# Reformat the glib library paths for Visual Studio
if(MSVC)
	foreach(LDFLAG ${GLIB_LDFLAGS} ${GOBJ_LDFLAGS})
		if(LDFLAG MATCHES "-l")
			string(REGEX REPLACE "^-l(.*)" "..\\\\extern\\\\glib-2.48.2\\\\${BUILD_PLATFORM}\\\\lib\\\\\\1.lib" LIBNAME ${LDFLAG})
			set(VS_GLIB ${VS_GLIB} ${LIBNAME})
		endif()
	endforeach()
	target_link_libraries(openxbox ${VS_GLIB})
else()
	target_link_libraries(openxbox ${GLIB_LDFLAGS} ${GOBJ_LDFLAGS})
endif()
target_include_directories(openxbox PUBLIC ${GLIB_INCLUDE_DIRS})

# Add custom commands to tell Visual Studio to copy the DLLs to the target directory
if(MSVC)
	add_custom_command(TARGET openxbox
		POST_BUILD
		COMMAND copy /b /y $(ProjectDir)..\\extern\\glew-2.1.0\\${BUILD_PLATFORM}\\bin\\glew32.dll $(TargetDir)
		COMMAND copy /b /y $(ProjectDir)..\\extern\\SDL2-2.0.7\\lib\\${BUILD_PLATFORM_ALT}\\SDL2.dll $(TargetDir)
		COMMAND copy /b /y $(ProjectDir)..\\extern\\unicorn-1.0.1\\${BUILD_PLATFORM}\\unicorn.dll $(TargetDir)
		COMMAND copy /b /y $(ProjectDir)..\\extern\\unicorn-1.0.1\\${BUILD_PLATFORM}\\libgcc_s_seh-1.dll $(TargetDir)
		COMMAND copy /b /y $(ProjectDir)..\\extern\\unicorn-1.0.1\\${BUILD_PLATFORM}\\libwinpthread-1.dll $(TargetDir)
		COMMAND copy /b /y $(ProjectDir)..\\extern\\glib-2.48.2\\${BUILD_PLATFORM}\\bin\\glib-2-vs10.dll $(TargetDir)
		COMMAND copy /b /y $(ProjectDir)..\\extern\\glib-2.48.2\\${BUILD_PLATFORM}\\bin\\gobject-2-vs10.dll $(TargetDir)
		COMMENT "Copy DLLs to target directory")
endif()