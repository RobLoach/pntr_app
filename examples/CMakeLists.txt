# CMAKE Modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

# Resources
file(GLOB resources resources/*)
set(examples_resources)
list(APPEND examples_resources ${resources})
file(COPY ${examples_resources} DESTINATION "resources/")

find_package(pntr)

# SDL
set(SDL_SHARED FALSE)
set(SDL2_DISABLE_UNINSTALL TRUE)
set(SDL2_DISABLE_INSTALL TRUE)
find_package(SDL2 REQUIRED)
set(SDL2_LIBRARIES SDL2::SDL2 SDL2::SDL2main)
add_executable(pntr_app_example_sdl
    pntr_app_example.c
)
target_link_libraries(pntr_app_example_sdl PUBLIC
    pntr
    pntr_app
    ${SDL2_LIBRARIES}
)
target_compile_definitions(pntr_app_example_sdl PUBLIC PNTR_APP_SDL)

# raylib
find_package(raylib QUIET)
add_executable(pntr_app_example_raylib
    pntr_app_example.c
)
target_link_libraries(pntr_app_example_raylib PUBLIC
    pntr
    pntr_app
    raylib
)
target_compile_definitions(pntr_app_example_raylib PUBLIC PNTR_APP_RAYLIB)

# libretro
find_package(libretro)
add_library(pntr_app_example_libretro SHARED
    pntr_app_example.c
)
target_link_libraries(pntr_app_example_libretro PUBLIC
    pntr
    pntr_app
    libretro
)
set_target_properties(pntr_app_example_libretro PROPERTIES PREFIX "")