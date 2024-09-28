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
