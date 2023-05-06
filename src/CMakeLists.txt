cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC_FILES
        "*.h"
        "*.hpp"
        "*.cpp"
        "*.cc"
        )

find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

message("Building with CMake version: ${CMAKE_VERSION}")

add_executable(${PROJECT_NAME} ${SRC_FILES})

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message("-- USE DEBUG SETUP --")
    target_compile_definitions(${PROJECT_NAME} PUBLIC IC_DEBUG_BUILD SPDLOG_NO_EXCEPTIONS)
else()
    message("-- USE RELEASE SETUP --")
    target_compile_definitions(${PROJECT_NAME} PUBLIC SPDLOG_NO_EXCEPTIONS)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        )

# copy config.ini
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/config.ini $<TARGET_FILE_DIR:${PROJECT_NAME}>)
