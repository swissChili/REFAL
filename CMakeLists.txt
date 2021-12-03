cmake_minimum_required(VERSION 3.20)
project(REFAL)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        REQUIRED)

add_executable(REFAL main.cpp Token.cpp Token.h Matcher.cpp Matcher.h VarContext.cpp VarContext.h)
target_link_libraries(REFAL
        Qt::Core
        )

