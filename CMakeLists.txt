cmake_minimum_required(VERSION 3.10)

# Prohibit in-source tree builds.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are strictly prohibited.")
endif()

include(cmake/Policies.cmake)

project(tipdump)

add_subdirectory(third_party/libpixel)

add_subdirectory(src/libtip)
add_subdirectory(src/tipdump)