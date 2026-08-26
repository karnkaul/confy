add_library(${package_name})
add_library(${package_name}::${package_name} ALIAS ${package_name})

target_compile_features(${package_name} PUBLIC cxx_std_23)

target_include_directories(${package_name} PRIVATE
  src
)

file(GLOB_RECURSE headers LIST_DIRECTORIES false "include/*.hpp")
target_sources(${package_name} PUBLIC FILE_SET HEADERS BASE_DIRS include FILES
  ${headers}
)

configure_file(src/build_version.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/include/${package_name}/build_version.hpp")
target_sources(${package_name} PUBLIC FILE_SET HEADERS BASE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/include" FILES
  "${CMAKE_CURRENT_BINARY_DIR}/include/${package_name}/build_version.hpp"
)

file(GLOB_RECURSE sources LIST_DIRECTORIES false "src/*.[hc]pp")
target_sources(${package_name} PRIVATE
  ${sources}
)

if(MINGW)
  target_link_libraries(${package_name} PRIVATE stdc++exp)
endif()
