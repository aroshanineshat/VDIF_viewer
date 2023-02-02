cmake_minimum_required(VERSION 3.20)
project(vdif_viewer)

set(SRC_DIR ./src)

add_executable(vdif_viewer ${SRC_DIR}/vdif_viewer.cpp  ${SRC_DIR}/vdif_cli.cpp)