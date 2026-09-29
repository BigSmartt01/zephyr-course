cmake_minimum_required(VERSION 3.20.0)
set(BOARD doit_esp32_devkit_v1/esp32/procpu)

list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Zephyr REQUIRED)
project(app)

target_sources(app PRIVATE src/main.cpp)

add_subdirectory(drivers/our_driver)