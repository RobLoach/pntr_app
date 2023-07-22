# CMAKE Modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

# Resources
file(GLOB resources resources/*)
set(examples_resources)
list(APPEND examples_resources ${resources})
file(COPY ${examples_resources} DESTINATION "resources/")

find_package(pntr)

if (EMSCRIPTEN)
    add_executable(pntr_app_example_emscripten
        pntr_app_example.c
    )
    target_link_libraries(pntr_app_example_emscripten PUBLIC
        pntr
        pntr_app
    )

    set_target_properties(pntr_app_example_emscripten PROPERTIES PREFIX "")
    set_target_properties(pntr_app_example_emscripten PROPERTIES SUFFIX ".html")
    set_target_properties(pntr_app_example_emscripten PROPERTIES OUTPUT_NAME "index")

    #target_link_options(pntr_app_example_emscripten PUBLIC "--preload-file resources@/resources --shell-file pntr_app_example.html")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file resources@/resources --shell-file ${CMAKE_SOURCE_DIR}/example/pntr_app_example.html")

    # Force disable the other examples
    set(PNTR_BUILD_EXAMPLE_SDL FALSE)
    set(PNTR_BUILD_EXAMPLE_RAYLIB FALSE)
    set(PNTR_BUILD_EXAMPLE_LIBRETRO FALSE)
endif()

# SDL
if (PNTR_BUILD_EXAMPLE_SDL)
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
endif()

# raylib
if (PNTR_BUILD_EXAMPLE_RAYLIB)
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
endif()

# libretro
if (PNTR_BUILD_EXAMPLE_LIBRETRO)
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
    target_compile_definitions(pntr_app_example_libretro PUBLIC PNTR_APP_LIBRETRO)
endif()