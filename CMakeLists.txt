cmake_minimum_required(VERSION 3.12.0)
project(GridMapCore)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
add_compile_options(-Wall -Wextra -Wpedantic)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## Define Eigen addons. Creates an interface library: CompileFlags
include(cmake/grid_map_core-extras.cmake)

## System dependencies are found with CMake's conventions
# Solution to find Eigen3 with Saucy.
find_package(Eigen3 QUIET)
if(NOT EIGEN3_FOUND)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(EIGEN3 REQUIRED eigen3)
  set(EIGEN3_INCLUDE_DIR ${EIGEN3_INCLUDE_DIRS})
endif()

find_package(Eigen3 REQUIRED)

###########
## Build ##
###########

## Declare a cpp library
add_library(GridMapCore
   src/GridMap.cpp
   src/GridMapMath.cpp
   src/SubmapGeometry.cpp
   src/BufferRegion.cpp
   src/Polygon.cpp
   src/CubicInterpolation.cpp
   src/iterators/GridMapIterator.cpp
   src/iterators/SubmapIterator.cpp
   src/iterators/CircleIterator.cpp
   src/iterators/EllipseIterator.cpp
   src/iterators/SpiralIterator.cpp
   src/iterators/PolygonIterator.cpp
   src/iterators/LineIterator.cpp
   src/iterators/SlidingWindowIterator.cpp
)
target_link_libraries(GridMapCore
  Eigen3::Eigen CompileFlags
)
target_include_directories(GridMapCore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:include>
)

##############
## Examples ##
##############
option(BUILD_EXAMLPES "Build examples" OFF)
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

#############
## Testing ##
#############

option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
  enable_testing()
  include(CTest)
  find_package(GTest REQUIRED)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
  add_library(test_helpers test/test_helpers.cpp)
    target_link_libraries(test_helpers
      GridMapCore
      ${GTEST_LIBRARIES}
      pthread
    )

  function(add_gtest_target target)
    add_executable(${target} test/${target}.cpp)
    target_link_libraries(${target}
      GridMapCore
      ${GTEST_LIBRARIES}
      pthread
      test_helpers
    )
    add_test(${target} "../tests/test_grid_map_core")
    target_include_directories(${target} PRIVATE include tests)
  endfunction()

  set(TEST_TARGETS
    test_grid_map_core
    CubicConvolutionInterpolationTest
    CubicInterpolationTest
    GridMapMathTest
    GridMapTest
    GridMapIteratorTest
    LineIteratorTest
    EllipseIteratorTest
    SubmapIteratorTest
    PolygonIteratorTest
    PolygonTest
    EigenPluginsTest
    SpiralIteratorTest
    SlidingWindowIteratorTest
  )

  foreach(target ${TEST_TARGETS})
    add_gtest_target(${target})
  endforeach()


  ###################
  ## Code_coverage ##
  ###################
  find_package(cmake_code_coverage QUIET)
  if(cmake_code_coverage_FOUND)
    add_gtest_coverage(
      TEST_BUILD_TARGETS
        GridMapCore-test
    )
  endif()
endif()

#################
## Clang_tools ##
#################
find_package(cmake_clang_tools QUIET)
if(cmake_clang_tools_FOUND)
  add_default_clang_tooling(
    DISABLE_CLANG_FORMAT
  )
endif(cmake_clang_tools_FOUND)

########################
## Installing targets ##
########################
# Export package for use from the build tree
set(CMAKE_EXPORT_DIR lib/cmake/GridMapCore)
set(TARGETS_NAME "GridMapCore")
set(TARGETS_NAMESPACE "GridMap")
set(TARGETS_CONFIG_FILENAME "${TARGETS_NAME}Config.cmake.in")

install(
  TARGETS "${TARGETS_NAME}" CompileFlags
  EXPORT  "${TARGETS_NAME}Targets"
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib 
)
install(EXPORT "${TARGETS_NAME}Targets" 
  NAMESPACE "${TARGETS_NAMESPACE}::"
  DESTINATION ${CMAKE_EXPORT_DIR}
)

# Install the include directory
install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/include/
  DESTINATION include
)
include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${TARGETS_CONFIG_FILENAME}" "${CMAKE_CURRENT_BINARY_DIR}/${TARGETS_NAME}Config.cmake"
  INSTALL_DESTINATION "${CMAKE_EXPORT_DIR}"
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGETS_NAME}Config.cmake" DESTINATION "${CMAKE_EXPORT_DIR}")

# Export without installing
export(
  EXPORT "${TARGETS_NAME}Targets"
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGETS_NAME}Targets.cmake"
  NAMESPACE "${TARGETS_NAMESPACE}::"
)

