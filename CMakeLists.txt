cmake_minimum_required(VERSION 3.16)

project(tinyLang LANGUAGES CXX)

# -std=c++20 (EXTENSIONS OFF keeps it from becoming -std=gnu++20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Mirrors `main.cpp src/*.cpp` from the Makefile.
# CONFIGURE_DEPENDS re-globs at build time so new files in src/ are picked up.
file(GLOB TINYLANG_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(tinyLang ${TINYLANG_SOURCES})

# -Isrc -I/usr/local/include
target_include_directories(tinyLang PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    /usr/local/include
)

# -L/usr/local/lib
target_link_directories(tinyLang PRIVATE
    /usr/local/lib
)
