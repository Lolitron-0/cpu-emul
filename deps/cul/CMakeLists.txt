cmake_minimum_required (VERSION 3.15)

project ("cul")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(${PROJECT_NAME} INTERFACE include/cul/cul.hpp)

target_include_directories(${PROJECT_NAME} INTERFACE include)

# add_executable(${PROJECT_NAME}_tests tests/tests.cpp)
#
# target_link_libraries(
# 	${PROJECT_NAME}_tests
# 	PRIVATE
#   GTest::gtest_main
# )
#
# include(GoogleTest)
# gtest_discover_tests(${PROJECT_NAME}_tests)
