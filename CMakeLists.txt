cmake_minimum_required(VERSION 3.3)
project(modelpp)

# Global configuration; dir for additional CMake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)

# Build in debug by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  message(WARNING "Setting default CMAKE_BUILD_TYPE to Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Global C++ configuration
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags to add (a lot) of warnings
add_compile_options(-Wall -Wextra -pedantic -fconcepts)

# Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Defines the ./include/ directory as being accessible for includes
# during compilation
include_directories(${CMAKE_SOURCE_DIR}/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  enable_testing()

  # Enable coverage
  include(CodeCoverage)

  if(LCOV_PATH)
    # Append the --coverage flag to compile and link options
    append_coverage_compiler_flags()

    # Exclude source code which we don't want to appear in the source coverage
    set(COVERAGE_EXCLUDES
      # Standard includes are generally located in /usr
      "/usr/*"
      # Conan provides this variable which points to the boost location
      "${CONAN_BOOST_ROOT}/*"
      # The actual test files
      "${CMAKE_SOURCE_DIR}/test/*"
      )
  else()
    message(WARNING "Coverage not enable; install lcov")
  endif()

  # Include the test subdirectory to the build
  add_subdirectory(${CMAKE_SOURCE_DIR}/test)

  if (LCOV_PATH)
    # Setup coverage using lcov
    # https://github.com/linux-test-project/lcov
    setup_target_for_coverage_lcov(
      NAME coverage
      EXECUTABLE make test || true
      )
  endif()
endif()

