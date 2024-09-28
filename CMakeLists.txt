# This file draws inspiration from:
# 1. https://github.com/banach-space/clang-tutor/blob/651b62285891346843e4649392d49216724d3d05/HelloWorld/CMakeLists.txt
# 2. https://github.com/coveooss/clang-tidy-plugin-examples/tree/47b2eec0bb31b8af095f33a326c75c7985c682c7

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(clang-tidy-hello-world LANGUAGES CXX)
find_package(LLVM REQUIRED)
find_package(Clang REQUIRED)
if("${LLVM_VERSION_MAJOR}" VERSION_LESS 18)
  message(FATAL_ERROR "Found LLVM ${LLVM_VERSION_MAJOR}, but need LLVM 18 or above")
endif()

# Use the same C++ standard as LLVM 18 does
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")

# LLVM is normally built without RTTI. Be consistent with that.
if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

add_library(AwesomePrefixCheck MODULE "")
target_include_directories(AwesomePrefixCheck
    PRIVATE
        ${CLANG_INCLUDE_DIRS}
        ${LLVM_INCLUDE_DIRS}
)
target_sources(AwesomePrefixCheck PRIVATE ${CMAKE_CURRENT_LIST_DIR}/AwesomePrefixCheck.cpp)
