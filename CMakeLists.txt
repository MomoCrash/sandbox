cmake_minimum_required(VERSION 3.31)
project(UltimateSimulator)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.inl ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/src/*.h ${PROJECT_SOURCE_DIR}/src/*.hpp)

add_executable(UltimateSimulator ${SOURCES} ${HEADERS})

target_include_directories(UltimateSimulator PUBLIC ${PROJECT_SOURCE_DIR}/src/game/includes)
target_include_directories(UltimateSimulator PUBLIC ${PROJECT_SOURCE_DIR}/src/threads/includes)
target_include_directories(UltimateSimulator PUBLIC ${PROJECT_SOURCE_DIR}/src/tools/includes)

target_compile_features(UltimateSimulator PRIVATE cxx_std_20)
target_link_libraries(UltimateSimulator PRIVATE SFML::Graphics)


add_custom_command(
        TARGET UltimateSimulator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$<TARGET_PROPERTY:UltimateSimulator,SOURCE_DIR>/resources"
        "$<TARGET_PROPERTY:${PROJECT_NAME},BINARY_DIR>/resources"
)
