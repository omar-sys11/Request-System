cmake_minimum_required(VERSION 3.16)

project(feedSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)


set(SOURCES
    src/main.cpp
    src/user.cpp
    src/RequestManager.cpp
    src/UserService.cpp
    src/createrequestwindow.cpp
    src/loginwindow.cpp
    src/dashboardwindow.cpp
)

add_executable(feedSystem ${SOURCES})

target_include_directories(feedSystem PRIVATE include)
target_link_libraries(feedSystem PRIVATE Qt6::Widgets)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(runTests
    tests/request_tests.cpp
    src/user.cpp
)

target_include_directories(runTests PRIVATE include)

target_link_libraries(runTests
    GTest::gtest_main
    GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(runTests)
