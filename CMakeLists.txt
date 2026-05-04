cmake_minimum_required(VERSION 3.16)

project(feedSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Network Sql)
find_package(GTest CONFIG REQUIRED)
find_package(Threads REQUIRED)

set(APP_HEADERS
        include/createrequestwindow.h
        include/dashboardwindow.h
        include/loginwindow.h
        include/OpenAiAPIClient.h
        include/Request.h
        include/RequestManager.h
        include/user.h
        include/UserService.h
        include/databasemanager.h
        include/networkclient.h
)

set_source_files_properties(${APP_HEADERS} PROPERTIES AUTOMOC ON)

set(APP_SOURCES
        src/main.cpp
        src/user.cpp
        src/UserService.cpp
        src/RequestManager.cpp
        src/createrequestwindow.cpp
        src/loginwindow.cpp
        src/dashboardwindow.cpp
        src/OpenAiAPIClient.cpp
        src/databasemanager.cpp
        src/networkclient.cpp
)

add_executable(feedSystem
        ${APP_SOURCES}
        ${APP_HEADERS}
)

target_include_directories(feedSystem
        PRIVATE
        include
)

target_link_libraries(feedSystem
        PRIVATE
        Qt6::Widgets
        Qt6::Core
        Qt6::Network
        Qt6::Sql
)

enable_testing()

set(TEST_SOURCES
        tests/request_tests.cpp
        src/user.cpp
        src/UserService.cpp
        src/RequestManager.cpp
)

add_executable(runTests
        ${TEST_SOURCES}
)

target_include_directories(runTests
        PRIVATE
        include
)

target_link_libraries(runTests
        PRIVATE
        GTest::gmock_main
        Qt6::Core
)

include(GoogleTest)
gtest_discover_tests(runTests)

add_executable(server
        src/server.cpp
)

target_link_libraries(server
        PRIVATE
        Threads::Threads
)