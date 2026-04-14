cmake_minimum_required(VERSION 3.16)
project(feedSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(feedSystem
    src/main.cpp
    src/createrequestwindow.cpp
    src/loginwindow.cpp
    src/dashboardwindow.cpp
    include/loginwindow.h
    include/dashboardwindow.h
    include/createrequestwindow.h
)

target_include_directories(feedSystem PRIVATE include)
target_link_libraries(feedSystem PRIVATE Qt6::Widgets)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(request_tests
    tests/request_tests.cpp
    src/user.cpp
)

target_include_directories(request_tests PRIVATE include)
target_link_libraries(request_tests PRIVATE GTest::gtest_main GTest::gmock)

include(GoogleTest)
gtest_discover_tests(request_tests)
