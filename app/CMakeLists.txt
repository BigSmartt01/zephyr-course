cmake_minimum_required(VERSION 3.20.0)
set(BOARD esp32s3_devkitc/esp32s3/procpu)

list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Zephyr REQUIRED)
project(app)

target_sources(app PRIVATE src/main.cpp)

add_subdirectory(drivers/our_driver)