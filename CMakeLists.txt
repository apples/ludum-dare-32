
find_package(SFML 2 REQUIRED window graphics system)

file(GLOB_RECURSE sources src/*)
add_executable(game ${sources})
target_include_directories(game PRIVATE ${SFML_INCLUDE_DIRS})
target_link_libraries(game ${SFML_LIBRARIES})
