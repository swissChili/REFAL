cmake_minimum_required(VERSION 3.15)
project(REFAL)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_executable(REFAL main.cpp Token.cpp Token.h Matcher.cpp Matcher.h
  VarContext.cpp VarContext.h Parser.cpp Parser.h AstNode.cpp
  AstNode.h Evaluator.cpp Evaluator.h Function.cpp Function.h Repl.cpp
  Repl.h PPrint.cpp PPrint.h StdLib.cpp StdLib.h)

target_link_libraries(REFAL Qt::Core readline)
