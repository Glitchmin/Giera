file(GLOB GIERA_TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/framework/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/framework/*.h)

add_executable(GieraTests ${GIERA_TEST_SOURCES})
target_include_directories(GieraTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/framework)
target_link_libraries(GieraTests PRIVATE giera_core SDL2::SDL2main)
add_dependencies(GieraTests giera_assets)

# One CTest entry per test file keeps the VS Code Testing panel readable
# while still letting you run a single suite.
file(GLOB GIERA_TEST_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Test*.cpp)
foreach(src ${GIERA_TEST_FILES})
    get_filename_component(name ${src} NAME_WE)
    string(REGEX REPLACE "^Test" "" suite ${name})
    add_test(NAME ${suite} COMMAND GieraTests --file ${name} WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endforeach()

# End-to-end: run the real game executable headless for 10 s of game time with
# the player auto-fighting. Fails if the loop crashes or never renders.
add_test(NAME GameSmokeTest COMMAND Giera --smoke-test 5 --autoplay WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set_tests_properties(GameSmokeTest PROPERTIES
    ENVIRONMENT "SDL_VIDEODRIVER=dummy"
    PASS_REGULAR_EXPRESSION "killed [1-9]"
    TIMEOUT 60)
