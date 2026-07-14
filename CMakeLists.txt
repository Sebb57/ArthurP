cmake_minimum_required(VERSION 3.20)

project(ArthurP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(COMPILE_FLAGS
    -Wall
    -Wextra
    -Werror
    -Ofast
    -flto=auto
)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
set(LIB_SOURCES ${SRC_FILES})
list(FILTER LIB_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")

add_library(core STATIC ${LIB_SOURCES})
target_include_directories(core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_options(core PRIVATE
    ${COMPILE_FLAGS}
)

add_executable(${PROJECT_NAME} src/main.cpp)
target_compile_options(core PRIVATE
    ${COMPILE_FLAGS}
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE core
)
set_target_properties(${PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)
