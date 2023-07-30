# CMAKE Modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

find_package(pntr)

add_executable(pntr_app_test
    pntr_app_test.c
)
target_compile_options(pntr_app_test PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion)
target_link_libraries(pntr_app_test PUBLIC
    pntr
    pntr_app
)
target_compile_definitions(pntr_app_test PUBLIC PNTR_APP_CLI)
set_property(TARGET pntr_app_test PROPERTY C_STANDARD 99)

list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
add_test(NAME pntr_app_test COMMAND pntr_app_test)