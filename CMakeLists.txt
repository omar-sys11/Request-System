cmake_minimum_required(VERSION 3.16)

project(feedSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()


set(APP_SOURCES
        src/main.cpp
        src/user.cpp
        src/UserService.cpp
        src/RequestManager.cpp
        src/createrequestwindow.cpp
        src/loginwindow.cpp
        src/dashboardwindow.cpp
)

qt_add_executable(feedSystem
        ${APP_SOURCES}
)

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

set(TEST_SOURCES
        tests/request_tests.cpp
        src/user.cpp
        src/RequestManager.cpp
)

add_executable(runTests ${TEST_SOURCES})

target_include_directories(runTests PRIVATE include)

target_link_libraries(runTests
        PRIVATE
        GTest::gtest
        GTest::gmock
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(runTests)
