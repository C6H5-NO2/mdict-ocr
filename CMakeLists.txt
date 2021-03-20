cmake_minimum_required(VERSION 3.8)

project(ocr_dict)

set(CMAKE_CXX_STANDARD 17)
set(ROOT_DIR "${CMAKE_SOURCE_DIR}")
message(STATUS "ROOT DIR: ${ROOT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "bin/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "bin/release")

add_executable(${PROJECT_NAME} "src/main.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE "${ROOT_DIR}/include")
target_sources(${PROJECT_NAME} PRIVATE "src/main.cpp")

set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${ROOT_DIR}")

target_link_libraries(${PROJECT_NAME} "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.18362.0/um/x64/WindowsApp.lib")
