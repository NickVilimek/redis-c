# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(ToyRedis)

# Architecture set
set(CMAKE_OSX_ARCHITECTURES arm64)

# Debugging congi 
set(CMAKE_BUILD_TYPE Debug)
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
endif()

# Locate and include GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# C++ version config 
set(CMAKE_CXX_STANDARD 14)

# Source files

set(COMMON
  src/ds/hashtable.h
  src/ds/hashtable.cpp
  src/helpers/hash.h
  src/helpers/hash.cpp
)

set(SOURCES
  # src/server.cpp
  # src/client.cpp
  src/helpers/utils.h
  src/helpers/utils.cpp
)

set (TESTSOURCES
  test/test_main.cpp
  test/test_hashtable.cpp
  test/test_hash.cpp
)

# Executable targets
add_executable(server "src/server" ${COMMON} ${SOURCES})
add_executable(client "src/client" ${COMMON} ${SOURCES})
add_executable(tests ${COMMON} ${TESTSOURCES})

target_link_libraries(
  tests 
  GTest::gtest_main
)

# Test Discovery
include(GoogleTest)
gtest_discover_tests(tests)
