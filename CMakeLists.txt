cmake_minimum_required(VERSION 4.3)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_DEBUG_POSTFIX "-d")

set(package_name confy)
project(${package_name} VERSION 0.1.1)

option(DCONF_BUILD_TESTS "Build confy tests" ${PROJECT_IS_TOP_LEVEL})

add_subdirectory(lib)

if (DCONF_BUILD_TESTS)
  if (NOT TARGET klib::klib-test-main)
    include(FetchContent)
    FetchContent_Declare(klib
      GIT_REPOSITORY "https://github.com/karnkaul/klib"
      GIT_TAG "v0.6.9"
      SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/klib"
    )
    set(KLIB_UNIT_TEST ON)
    FetchContent_MakeAvailable(klib)
  endif()

  enable_testing()
  add_subdirectory(tests)
endif()
