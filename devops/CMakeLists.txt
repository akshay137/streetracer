cmake_minimum_required(VERSION 3.16)
project(sracer)

set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_BUILD_RPATH_USE_ORIGIN true)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "dist")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(KATHA_XR "Enable OpenXR support" ON)
option(KATHA_PRINT_LOG_DIAGNOSTICS "Appends profiling data for each log_line" ON)

set(BUILD_ROOT "..")
get_filename_component(BUILD_ROOT "${BUILD_ROOT}" ABSOLUTE)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC "${BUILD_ROOT}/ext")

target_compile_definitions(${PROJECT_NAME} PUBLIC KATHA_LINUX=0)
target_compile_definitions(${PROJECT_NAME} PUBLIC KATHA_WINDOWS=2)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions(${PROJECT_NAME} PUBLIC KATHA_PLATFORM=KATHA_LINUX)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC m)

set(SDL_TEST_LIBRARY OFF)
set(SDL_CAMERA OFF)
add_subdirectory("${BUILD_ROOT}/ext/SDL2" "${CMAKE_BINARY_DIR}/ext" EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)

if (KATHA_PRINT_LOG_DIAGNOSTICS)
	target_compile_definitions(${PROJECT_NAME} PUBLIC KATHA_PRINT_LOG_DIAGNOSTICS=1)
else()
	target_compile_definitions(${PROJECT_NAME} PUBLIC KATHA_PRINT_LOG_DIAGNOSTICS=0)
endif()

if (KATHA_XR)
	target_compile_definitions(${PROJECT_NAME} PUBLIC KATHA_XR=1)
	target_link_libraries(${PROJECT_NAME} PUBLIC openxr_loader)
	target_sources(${PROJECT_NAME} PUBLIC
		${BUILD_ROOT}/source/platform/xr/xr.hpp
		${BUILD_ROOT}/source/platform/xr/xr.cpp
		${BUILD_ROOT}/source/platform/xr/xr.gl.cpp
	)
else()
	target_compile_definitions(${PROJECT_NAME} PUBLIC KATHA_XR=0)
endif()

target_sources(${PROJECT_NAME} PUBLIC
	${BUILD_ROOT}/source/platform/main.cpp
	${BUILD_ROOT}/source/platform/platform.hpp
	${BUILD_ROOT}/source/platform/platform.cpp
	${BUILD_ROOT}/source/platform/gamepad.hpp
	${BUILD_ROOT}/source/platform/gamepad.cpp
	${BUILD_ROOT}/source/platform/input.hpp
	${BUILD_ROOT}/source/platform/log.cpp
	${BUILD_ROOT}/source/platform/memory.cpp
	${BUILD_ROOT}/source/platform/file.hpp
	${BUILD_ROOT}/source/platform/file.cpp
	${BUILD_ROOT}/source/platform/glad.c

	${BUILD_ROOT}/source/platform/gl/gl.hpp
	${BUILD_ROOT}/source/platform/gl/gl.cpp

	${BUILD_ROOT}/source/platform/vulkan/vulkan_context.hpp
	${BUILD_ROOT}/source/platform/vulkan/vulkan_context.cpp

	${BUILD_ROOT}/source/katha/core.hpp
	${BUILD_ROOT}/source/katha/core/config.hpp
	${BUILD_ROOT}/source/katha/core/config.cpp
	${BUILD_ROOT}/source/katha/core/enums.hpp
	${BUILD_ROOT}/source/katha/core/enums.cpp
	${BUILD_ROOT}/source/katha/core/stream.hpp
	${BUILD_ROOT}/source/katha/core/string.hpp
	${BUILD_ROOT}/source/katha/core/string.cpp
	${BUILD_ROOT}/source/katha/core/types.hpp
	
	${BUILD_ROOT}/source/katha/format/string.hpp
	${BUILD_ROOT}/source/katha/format/string.cpp

	${BUILD_ROOT}/source/katha/graphics/vertex.hpp
	
	${BUILD_ROOT}/source/katha/math/vector2.hpp
	${BUILD_ROOT}/source/katha/math/vector3.hpp
	${BUILD_ROOT}/source/katha/math/vector3.hpp
	${BUILD_ROOT}/source/katha/math/quaternion.hpp
	${BUILD_ROOT}/source/katha/math/matrix4.hpp

	${BUILD_ROOT}/source/katha/physics/transform.hpp
	${BUILD_ROOT}/source/katha/physics/transform.cpp

	${BUILD_ROOT}/source/katha/time/time.hpp
	${BUILD_ROOT}/source/katha/time/time.cpp
)