cmake_minimum_required(VERSION 3.0.0)
project(compiler VERSION 0.1.0 LANGUAGES C)

add_executable(compiler main.c lexer.c parser.c analyzer.c generator.c)