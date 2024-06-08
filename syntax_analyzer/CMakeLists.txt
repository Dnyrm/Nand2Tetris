cmake_minimum_required(VERSION 3.28)
project(syntax_analyzer)

set(CMAKE_CXX_STANDARD 17)

add_executable(syntax_analyzer main.cpp
        JackTokenizer.cpp
        JackTokenizer.h
        TokenType.h
        KeyWord.h
        CompilationEngine.h
        JackAnalyzer.h
        JackAnalyzer.cpp
        CompilationEngine.cpp
)
