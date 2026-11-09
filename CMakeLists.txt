cmake_minimum_required(VERSION 3.20)

project(ProjectJarvis VERSION 0.1.0 LANGUAGES CXX)

option(JARVIS_BUILD_TESTS "Build Jarvis native tests" ON)

add_library(jarvis_core
    cpp/src/tool_registry.cpp
    cpp/src/builtin_tools.cpp
)

target_include_directories(jarvis_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cpp/include)
target_compile_features(jarvis_core PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(jarvis_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(jarvis_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(jarvis cpp/src/main.cpp)
target_link_libraries(jarvis PRIVATE jarvis_core)

if(JARVIS_BUILD_TESTS)
    enable_testing()
    add_executable(jarvis_tool_tests cpp/tests/test_tool_registry.cpp)
    target_link_libraries(jarvis_tool_tests PRIVATE jarvis_core)
    add_test(NAME jarvis_tool_tests COMMAND jarvis_tool_tests)
endif()
