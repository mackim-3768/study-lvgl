cmake_minimum_required(VERSION 3.16)
project(watch_app C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wno-unused-parameter)

# ---------------------------------------------------------------------------
# LVGL via FetchContent
# ---------------------------------------------------------------------------
include(FetchContent)

set(LV_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lv_conf.h" CACHE STRING "" FORCE)
set(LV_CONF_INCLUDE_SIMPLE ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    lvgl
    GIT_REPOSITORY https://github.com/lvgl/lvgl.git
    GIT_TAG        v9.2.2
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(lvgl)

# ---------------------------------------------------------------------------
# SDL2
# ---------------------------------------------------------------------------
find_package(SDL2 REQUIRED)

# Some SDL2 installations on macOS (Homebrew) don't set SDL2_INCLUDE_DIRS.
if(NOT SDL2_INCLUDE_DIRS AND SDL2_INCLUDE_DIR)
    set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
endif()

# ---------------------------------------------------------------------------
# Application target
# ---------------------------------------------------------------------------
add_executable(watch_app
    main.c
    src/stopwatch_ui.c
)

target_include_directories(watch_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(watch_app PRIVATE
    lvgl
    lvgl::examples
    ${SDL2_LIBRARIES}
)
