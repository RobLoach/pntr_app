# CMAKE Modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

# Resources
file(GLOB resources resources/*)
set(examples_resources)
list(APPEND examples_resources ${resources})
file(COPY ${examples_resources} DESTINATION "resources/")

find_package(pntr)

# SDL or Emscripten
if (PNTR_APP_BUILD_EXAMPLE_SDL AND NOT EMSCRIPTEN)
    add_executable(pntr_app_example_sdl
        pntr_app_example.c
    )

    target_compile_definitions(pntr_app_example_sdl PUBLIC PNTR_APP_SDL)

    # Dependencies
    set(SDL_SHARED FALSE)
    set(SDL2_DISABLE_UNINSTALL TRUE)
    set(SDL2_DISABLE_INSTALL TRUE)
    find_package(SDL2 REQUIRED)
    set(SDL2_LIBRARIES SDL2::SDL2 SDL2::SDL2main)
    target_link_libraries(pntr_app_example_sdl PUBLIC
        pntr
        pntr_app
        ${SDL2_LIBRARIES}
    )
    set_property(TARGET pntr_app_example_sdl PROPERTY C_STANDARD 99)
endif()

# raylib
if (PNTR_APP_BUILD_EXAMPLE_RAYLIB)
    find_package(raylib QUIET)
    add_executable(pntr_app_example_raylib
        pntr_app_example.c
    )
    target_compile_definitions(pntr_app_example_raylib PUBLIC PNTR_APP_RAYLIB)

    # Platform Updates
    if (EMSCRIPTEN)
        set_target_properties(pntr_app_example_raylib PROPERTIES OUTPUT_NAME "index")
        set_target_properties(pntr_app_example_raylib PROPERTIES SUFFIX ".html")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY --preload-file resources@/resources --shell-file ${CMAKE_SOURCE_DIR}/example/pntr_app_example.html")
    else()
        set_property(TARGET pntr_app_example_raylib PROPERTY C_STANDARD 99)
    endif()

    if (APPLE AND NOT EMSCRIPTEN)
        target_link_libraries(pntr_app_example_raylib "-framework IOKit")
        target_link_libraries(pntr_app_example_raylib "-framework Cocoa")
        target_link_libraries(pntr_app_example_raylib "-framework OpenGL")
    endif()

    target_link_libraries(pntr_app_example_raylib PUBLIC
        pntr
        pntr_app
        raylib
    )
endif()

# libretro
if (PNTR_APP_BUILD_EXAMPLE_LIBRETRO AND NOT EMSCRIPTEN)
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
    set_property(TARGET pntr_app_example_libretro PROPERTY C_STANDARD 99)
endif()

# CLI Example
#add_executable(pntr_app_example_cli pntr_app_example.c)
#target_link_libraries(pntr_app_example_cli PUBLIC pntr pntr_app)
# target_compile_definitions(pntr_app_example_cli PUBLIC PNTR_APP_CLI)
# set_property(TARGET pntr_app_example_cli PROPERTY C_STANDARD 99)
