cmake_minimum_required(VERSION 3.28)
project(PS VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch SFML
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)

add_executable(${PROJECT_NAME} main.cpp
        ParticleSystem.h
        ReactiveRecorder.h
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)

if (WIN32 AND BUILD_SHARED_LIBS)            # only when we’re using DLLs
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND_EXPAND_LISTS)               # ← expands the DLL list
endif()
