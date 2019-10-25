cmake_minimum_required(VERSION 3.5.1)
project(lab0 C)

set(SRC src/main.c src/TransferSN.c src/TransferSN.h src/CharAndNumTransfer.c src/CharAndNumTransfer.h src/CheckInput.c src/CheckInput.h src/globalConsts.h src/SequenceFunctions.c src/SequenceFunctions.h)

add_executable(${PROJECT_NAME} ${SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
