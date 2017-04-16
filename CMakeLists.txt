cmake_minimum_required(VERSION 2.6)
project(aogl)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(tests)

# search for dependencies
if (MSVC)
	find_library(GLFW_LIBRARY glfw3.lib PATHS lib/bin)
	if (NOT GLFW_LIBRARY)
		message("couldn't find glfw")
	endif ()
else ()
	find_library(GLFW_LIBRARY libglfw3.a PATHS lib/bin)
	if (NOT GLFW_LIBRARY)
		message("couldn't find glfw")
	endif()
endif()

if (MSVC)
	find_library(GLEW_LIBRARY glew32s.lib PATHS lib/bin)
	if (NOT GLEW_LIBRARY)
		message("couldn't find glew")
	endif ()
else ()
	find_library(GLEW_LIBRARY libglew32.a PATHS lib/bin)
	if (NOT GLEW_LIBRARY)
		message("couldn't find glew")
	endif()
endif()

if (MSVC) 
	find_library(SOIL_LIBRARY SOIL.lib PATHs lib/bin)
	if (NOT SOIL_LIBRARY)
		message("couldn't find SOIL")
	endif()
else ()
	find_library(SOIL_LIBRARY libsoil.a PATHs lib/bin)
	if (NOT SOIL_LIBRARY)
		message("couldn't find SOIL")
	endif()
endif ()

set(LIB_INCLUDES ${CMAKE_SOURCE_DIR}/lib/include)

set(AOGL_DEPENDENCIES ${GLFW_LIBRARY} ${GLEW_LIBRARY} ${SOIL_LIBRARY} opengl32.lib)


set(SOURCE_FILES src/buffer.cpp src/framebuffer.cpp src/gl.cpp src/image.cpp src/mesh.cpp src/program.cpp src/shader.cpp src/stdafx.cpp src/texture.cpp src/vertex_array.cpp src/window.cpp)
add_library(aogl STATIC ${SOURCE_FILES})

target_link_libraries(aogl ${AOGL_DEPENDENCIES})
target_include_directories(aogl PUBLIC ${LIB_INCLUDES})
