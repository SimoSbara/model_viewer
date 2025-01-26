cmake_minimum_required(VERSION 3.5.0)
project(model_viewer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(OSBit 64)
else()
	set(OSBit 32)
endif()

set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(BASE_INSTALL_DIR ${BIN_DIR}/x${OSBit}/${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE model_viewer_src
     "${PROJECT_SOURCE_DIR}/src/*.h"
     "${PROJECT_SOURCE_DIR}/src/*.hpp"
     "${PROJECT_SOURCE_DIR}/src/*.cpp"
     "${PROJECT_SOURCE_DIR}/src/*.c"
)

add_library(model_viewer_lib STATIC ${model_viewer_src})
add_executable(model_viewer "${PROJECT_SOURCE_DIR}/src/ModelViewer.cpp")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(model_viewer
	${PROJECT_SOURCE_DIR}/externals/GLFW/include
	${PROJECT_SOURCE_DIR}/externals/glm/
	${PROJECT_SOURCE_DIR}/include/
)

add_subdirectory(${PROJECT_SOURCE_DIR}/externals/GLFW)
add_subdirectory(${PROJECT_SOURCE_DIR}/externals/glm)

find_package(OpenGL REQUIRED)

target_link_libraries(model_viewer
	glfw 
	glm 
	OpenGL::GL
	model_viewer_lib
)

install(TARGETS model_viewer DESTINATION ${BASE_INSTALL_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/shaders
                ${BASE_INSTALL_DIR}/shaders)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/models
                ${BASE_INSTALL_DIR}/models)