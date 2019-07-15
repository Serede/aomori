cmake_minimum_required(VERSION 3.0.0)
project(aomori VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
include(CTest)
enable_testing()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_TESTING_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test)

include_directories(include)
add_subdirectory(src)
