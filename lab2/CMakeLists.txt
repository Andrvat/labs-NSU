cmake_minimum_required(VERSION 3.5.1)
project(lab2 C)

set(SRC src/main.c src/OperationsArrayOfChar.c src/AlgorithmOfPermutation.c)

add_executable(${PROJECT_NAME} ${SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")