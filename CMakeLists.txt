cmake_minimum_required(VERSION 3.16)

project(FeedSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/main.cpp

    src/FeedWindow.cpp
    src/FeedBoard.cpp

    src/FeedItem.cpp
    src/RequestItem.cpp
    src/MessageItem.cpp
    src/AcceptItem.cpp
)

set(HEADERS
    include/FeedWindow.h
    include/FeedBoard.h

    include/FeedItem.h
    include/RequestItem.h
    include/MessageItem.h
    include/AcceptItem.h
)

add_executable(FeedSystem
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(FeedSystem PRIVATE include)

target_link_libraries(FeedSystem PRIVATE Qt6::Widgets)
