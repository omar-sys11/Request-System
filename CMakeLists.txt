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
        src/RequestService.cpp
        src/createrequestwindow.cpp
        src/loginwindow.cpp
        src/dashboardwindow.cpp
)

set(HEADERS
        include/loginwindow.h
        include/dashboardwindow.h
        include/createrequestwindow.h
        include/RequestManager.h
        include/RequestManager.h

)

add_executable(feedSystem
        ${SOURCES}
        ${HEADERS}
        include/loginwindow.h
)

target_include_directories(feedSystem PRIVATE include)

target_link_libraries(feedSystem PRIVATE Qt6::Widgets)
