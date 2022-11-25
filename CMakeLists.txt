cmake_minimum_required(VERSION 3.23)
project(MorzeNHF C)

set(CMAKE_C_STANDARD 99)

add_executable(MorzeNHF main.c cli.c cli.h tree.c tree.h TreeNode.h file.c file.h)
