
find_package(SFML 2 REQUIRED window graphics system)

file(GLOB_RECURSE sources src/*)
add_executable(game ${sources})
set_property(TARGET game PROPERTY CXX_STANDARD 11)
target_include_directories(game PRIVATE ${SFML_INCLUDE_DIRS})
target_link_libraries(game ${SFML_LIBRARIES})
