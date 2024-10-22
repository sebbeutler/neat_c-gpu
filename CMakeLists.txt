cmake_minimum_required(VERSION 3.10.0)

project(NEAT_C-GPU C)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_BUILD_TYPE Debug)

# binary folder after building
set(BIN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${BIN_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BIN_OUTPUT_DIR})

# source code at root
set(SOURCES
    ${PROJECT_SOURCE_DIR}/main.c
    ${PROJECT_SOURCE_DIR}/format_io.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

# includes
target_include_directories(${PROJECT_NAME} PRIVATE ./)

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})
target_link_libraries (${PROJECT_NAME} ${OpenCL_LIBRARY})

# Copy kernels to the binary dir
# configure_file(${PROJECT_SOURCE_DIR}/kernels/kernel.cl ${BIN_OUTPUT_DIR}/kernel.cl COPYONLY)
# file(COPY ${PROJECT_SOURCE_DIR}/kernels/kernel.cl DESTINATION ${BIN_OUTPUT_DIR})

add_custom_command(
        TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
            ${PROJECT_SOURCE_DIR}/kernels/
            ${BIN_OUTPUT_DIR}/kernels/)

# Link the math library, needed on UNIX systems.
if (UNIX)
    target_link_libraries(${PROJECT_NAME} m)
endif (UNIX)

# disable unsafe functions warnings
# add_compile_options(/w44101)
if (WIN32)
    set(CMAKE_C_FLAGS "/D_CRT_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_DEPRECATE ${CMAKE_C_FLAGS}")
    set(CMAKE_C_FLAGS "/D_CRT_NONSTDC_NO_DEPRECATE /D_SCL_SECURE_NO_DEPRECATE ${CMAKE_C_FLAGS}")
endif (WIN32)

# disable ZERO_CHECK project
set(CMAKE_SUPPRESS_REGENERATION true)