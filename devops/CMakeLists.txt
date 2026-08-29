cmake_minimum_required(VERSION 3.16)
project(sracer)

set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDAR 17)

set(CMAKE_BUILD_RPATH_USE_ORIGIN true)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "dist")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dist")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(KATHA_XR "Enable OpenXR support" ON)
option(KATHA_PRINT_LOG_DIAGNOSTICS "Appends profiling data for each log_line" ON)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	option(KATHA_LINUX "Linux Platform" ON)
	option(KATHA_WINDOWS "Windows Platform" OFF)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	option(KATHA_WINDOWS "Windows Platform" ON)
	option(KATHA_LINUX "Linux Platform" OFF)
endif()

set(BUILD_ROOT "..")
get_filename_component(BUILD_ROOT "${BUILD_ROOT}" ABSOLUTE)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC "${BUILD_ROOT}/ext")

target_compile_definitions(${PROJECT_NAME} PUBLIC KATHA_LINUX=0)
target_compile_definitions(${PROJECT_NAME} PUBLIC KATHA_WINDOWS=2)

if (KATHA_LINUX)
	target_compile_definitions(${PROJECT_NAME} PUBLIC KATHA_PLATFORM=KATHA_LINUX)
elseif(KATHA_WINDOWS)
	target_compile_definitions(${PROJECT_NAME} PUBLIC KATHA_PLATFORM=KATHA_WINDOWS)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC m)

set(SDL_TEST_LIBRARY OFF)
set(SDL_CAMERA OFF)
set(SDL_STATIC ON)
add_subdirectory("${BUILD_ROOT}/ext/SDL2" "${CMAKE_BINARY_DIR}/ext" EXCLUDE_FROM_ALL)
if (KATHA_WINDOWS)
	target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2main)
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)

if (KATHA_WINDOWS)
	target_link_libraries(${PROJECT_NAME} PRIVATE -static-libstdc++)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
		$<TARGET_FILE_DIR:${PROJECT_NAME}>
		COMMAND_EXPAND_LISTS
	)
endif()

if (KATHA_PRINT_LOG_DIAGNOSTICS)
	target_compile_definitions(${PROJECT_NAME} PUBLIC KATHA_PRINT_LOG_DIAGNOSTICS=1)
else()
	target_compile_definitions(${PROJECT_NAME} PUBLIC KATHA_PRINT_LOG_DIAGNOSTICS=0)
endif()

if (KATHA_XR)
	target_compile_definitions(${PROJECT_NAME} PUBLIC KATHA_XR=1)
	target_link_libraries(${PROJECT_NAME} PUBLIC openxr_loader)
	target_sources(${PROJECT_NAME} PUBLIC
		${BUILD_ROOT}/source/graphics/xr/xr.hpp
		${BUILD_ROOT}/source/graphics/xr/xr.cpp
		${BUILD_ROOT}/source/graphics/xr/xr.gl.cpp
	)
else()
	target_compile_definitions(${PROJECT_NAME} PUBLIC KATHA_XR=0)
endif()

target_sources(${PROJECT_NAME} PUBLIC
	${BUILD_ROOT}/source/config.cpp
	${BUILD_ROOT}/source/config.hpp
	${BUILD_ROOT}/source/constants.hpp
	${BUILD_ROOT}/source/core.hpp
	${BUILD_ROOT}/source/utility.cpp
	${BUILD_ROOT}/source/utility.hpp

	${BUILD_ROOT}/source/enum/base.cpp
	${BUILD_ROOT}/source/enum/base.hpp
	${BUILD_ROOT}/source/enum/features.hpp
	${BUILD_ROOT}/source/enum/format.cpp
	${BUILD_ROOT}/source/enum/format.hpp
	${BUILD_ROOT}/source/enum/graphics.cpp
	${BUILD_ROOT}/source/enum/graphics.hpp
	${BUILD_ROOT}/source/enum/order.hpp
	${BUILD_ROOT}/source/enum/result.cpp
	${BUILD_ROOT}/source/enum/result.hpp

	${BUILD_ROOT}/source/format/string.cpp
	${BUILD_ROOT}/source/format/string.hpp
	${BUILD_ROOT}/source/format/types.cpp
	${BUILD_ROOT}/source/format/types.hpp

	${BUILD_ROOT}/source/game/vehicle.cpp
	${BUILD_ROOT}/source/game/vehicle.hpp
	${BUILD_ROOT}/source/game/world.cpp
	${BUILD_ROOT}/source/game/world.hpp

	${BUILD_ROOT}/source/graphics/buffer.hpp
	${BUILD_ROOT}/source/graphics/framebuffer.hpp
	${BUILD_ROOT}/source/graphics/graphics.hpp
	${BUILD_ROOT}/source/graphics/mesh.hpp
	${BUILD_ROOT}/source/graphics/pso.hpp
	${BUILD_ROOT}/source/graphics/texture.hpp

	${BUILD_ROOT}/source/graphics/gl/glad.c
	${BUILD_ROOT}/source/graphics/gl/glad/glad.h
	${BUILD_ROOT}/source/graphics/gl/gl.cpp
	${BUILD_ROOT}/source/graphics/gl/gl.graphics.cpp
	${BUILD_ROOT}/source/graphics/gl/gl.graphics.render.cpp
	${BUILD_ROOT}/source/graphics/gl/gl.hpp
	${BUILD_ROOT}/source/graphics/gl/KHR/khrplatform.h

	${BUILD_ROOT}/source/graphics/vulkan/vulkan_context.cpp
	${BUILD_ROOT}/source/graphics/vulkan/vulkan_context.hpp
	
	${BUILD_ROOT}/source/math/matrix4.hpp
	${BUILD_ROOT}/source/math/quaternion.hpp
	${BUILD_ROOT}/source/math/utility.hpp
	${BUILD_ROOT}/source/math/vector2.gen.hpp
	${BUILD_ROOT}/source/math/vector2.hpp
	${BUILD_ROOT}/source/math/vector3.gen.hpp
	${BUILD_ROOT}/source/math/vector3.hpp
	${BUILD_ROOT}/source/math/vector4.gen.hpp
	${BUILD_ROOT}/source/math/vector4.hpp

	${BUILD_ROOT}/source/platform/input.hpp
	${BUILD_ROOT}/source/platform/main.cpp
	${BUILD_ROOT}/source/platform/platform.cpp
	${BUILD_ROOT}/source/platform/platform.hpp

	${BUILD_ROOT}/source/resource/resource.hpp
	${BUILD_ROOT}/source/resource/resource.mesh.cpp

	${BUILD_ROOT}/source/time/time.cpp
	${BUILD_ROOT}/source/time/time.hpp

	${BUILD_ROOT}/source/type/action_map.hpp
	${BUILD_ROOT}/source/type/file.cpp
	${BUILD_ROOT}/source/type/file.hpp
	${BUILD_ROOT}/source/type/flags.hpp
	${BUILD_ROOT}/source/type/gamepad.cpp
	${BUILD_ROOT}/source/type/gamepad.hpp
	${BUILD_ROOT}/source/type/matrix.hpp
	${BUILD_ROOT}/source/type/primitive.hpp
	${BUILD_ROOT}/source/type/quaternion.hpp
	${BUILD_ROOT}/source/type/stack_string.hpp
	${BUILD_ROOT}/source/type/stream.hpp
	${BUILD_ROOT}/source/type/string.cpp
	${BUILD_ROOT}/source/type/string.hpp
	${BUILD_ROOT}/source/type/transform.cpp
	${BUILD_ROOT}/source/type/transform.hpp
	${BUILD_ROOT}/source/type/vector.hpp
	${BUILD_ROOT}/source/type/version.hpp
	${BUILD_ROOT}/source/type/vertex.hpp
)

# exclude c++ runtime
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(${PROJECT_NAME} PUBLIC
		-fno-exceptions
		-fno-rtti
		-fno-threadsafe-statics
		-fno-use-cxa-atexit
	)

	target_link_options(${PROJECT_NAME} PUBLIC
		"-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/linker.map"
		"-Wl,--cref"
	)

	set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(${PROJECT_NAME} PUBLIC
		-fno-exceptions
		-fno-rtti
		-fno-threadsafe-statics
		-fno-use-cxa-atexit
	)
	
	target_link_options(${PROJECT_NAME} PUBLIC -nostdlibc++)
endif()