# CMAKE Modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

# Resources
file(GLOB resources resources/*)
set(examples_resources)
list(APPEND examples_resources ${resources})
file(COPY ${examples_resources} DESTINATION "resources/")

find_package(pntr)

set(BUILD_SHARED_LIBS FALSE)

# SDL
if (PNTR_APP_BUILD_EXAMPLE_SDL AND NOT EMSCRIPTEN)
    add_executable(pntr_app_example_sdl
        pntr_app_example.c
    )

    target_compile_definitions(pntr_app_example_sdl PUBLIC PNTR_APP_SDL)

    # SDL3_mixer
    set(SDLMIXER_BUILD_SHARED_LIBS false)

    # SDL
    #set(SDL_SHARED FALSE)
    #set(SDL2_SHARED FALSE)
    #set(SDL_STATIC TRUE)
    #set(SDL_TEST FALSE)
    #set(SDL_TESTS FALSE)
    #set(INTERFACE_SDL2_SHARED false)
    #set(SDL2_DISABLE_UNINSTALL TRUE)
    #set(SDL2_DISABLE_INSTALL TRUE)
    #set(SDL_INSTALL_TESTS FALSE)
    find_package(SDL3 REQUIRED)
    set(PNTR_SDL_LIBRARIES SDL3::SDL3 SDL3_mixer-static)

    # SDL_mixer
    set(SDLMIXER_VORBIS STB)
    set(SDLMIXER_INSTALL OFF)
    set(SDLMIXER_DEPS_SHARED OFF)
    set(SDLMIXER_VENDORED OFF)
    set(SDLMIXER_SAMPLES OFF)
    set(SDLMIXER_CMD OFF)
    set(SDLMIXER_FLAC OFF)
    set(SDLMIXER_MOD OFF)
    set(SDLMIXER_MP3 OFF)
    set(SDLMIXER_MIDI_NATIVE OFF)
    set(SDLMIXER_OPUS OFF)
    set(SDLMIXER_OPUS_SHARED OFF)
    set(SDLMIXER_MIDI_FLUIDSYNTH OFF)
    set(SDLMIXER_MP3_MPG123 OFF)
    set(SDLMIXER_MP3_DRMP3 OFF)
    set(SDLMIXER_MOD_XMP OFF)
    set(SDLMIXER_MOD_MODPLUG OFF)
    set(SDLMIXER_FLAC_DRFLAC OFF)
    set(SDLMIXER_FLAC_LIBFLAC OFF)
    set(SDLMIXER_VORBIS_VORBISFILE OFF)
    set(SDLMIXER_WAVPACK OFF)
    set(SDLMIXER_SAMPLES_INSTALL OFF)
    set(SDLMIXER_BUILD_SHARED_LIBS OFF)
    find_package(SDL3_mixer REQUIRED)
    set(SDL2_LIBRARIES ${SDL2_LIBRARIES} SDL2_mixer::SDL2_mixer-static)
    #set(SDL2_LIBRARIES ${SDL2_LIBRARIES} SDL2_mixer)

    target_compile_definitions(pntr_app_example_sdl PUBLIC
        PNTR_APP_SDL_MIXER
    )

    target_link_libraries(pntr_app_example_sdl PUBLIC
        pntr
        pntr_app
        ${PNTR_SDL_LIBRARIES}
    )
    
    set_property(TARGET pntr_app_example_sdl PROPERTY C_STANDARD 11)
    # Strict Warnings and Errors
    if(MSVC)
        target_compile_options(pntr_app_example_sdl PRIVATE /W4 /WX)
    else()
        target_compile_options(pntr_app_example_sdl PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()

endif()

# raylib or Emscripten
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
        set_property(TARGET pntr_app_example_raylib PROPERTY C_STANDARD 11)
    endif()

    if (APPLE AND NOT EMSCRIPTEN)
        target_link_libraries(pntr_app_example_raylib PUBLIC "-framework IOKit")
        target_link_libraries(pntr_app_example_raylib PUBLIC "-framework Cocoa")
        target_link_libraries(pntr_app_example_raylib PUBLIC "-framework OpenGL")
    endif()

    target_link_libraries(pntr_app_example_raylib PUBLIC
        pntr
        pntr_app
        raylib_static
    )
    # Strict Warnings and Errors
    if(MSVC)
        target_compile_options(pntr_app_example_raylib PRIVATE /W4 /WX)
    else()
        target_compile_options(pntr_app_example_raylib PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()

endif()

# CLI
if (PNTR_APP_BUILD_EXAMPLE_CLI AND NOT EMSCRIPTEN)
    add_executable(pntr_app_example_cli pntr_app_example.c)
    target_link_libraries(pntr_app_example_cli PUBLIC pntr pntr_app)
    target_compile_definitions(pntr_app_example_cli PUBLIC PNTR_APP_CLI)
    set_property(TARGET pntr_app_example_cli PROPERTY C_STANDARD 11)
    # Strict Warnings and Errors
    if(MSVC)
        target_compile_options(pntr_app_example_cli PRIVATE /W4 /WX)
    else()
        target_compile_options(pntr_app_example_cli PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()

endif()


# Emscripten
if (PNTR_APP_BUILD_EXAMPLE_WEB AND EMSCRIPTEN)
    add_executable(pntr_app_example_web pntr_app_example.c)
    target_link_libraries(pntr_app_example_web PUBLIC pntr pntr_app)
    target_compile_definitions(pntr_app_example_web PUBLIC PNTR_APP_WEB)
    set_property(TARGET pntr_app_example_web PROPERTY C_STANDARD 11)
    set_target_properties(pntr_app_example_web PROPERTIES SUFFIX ".html")
    set_target_properties(pntr_app_example_web PROPERTIES OUTPUT_NAME "index")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASSERTIONS=1 -s WASM=1 --preload-file resources@/resources --shell-file ${CMAKE_SOURCE_DIR}/example/pntr_app_example.html")
    # Strict Warnings and Errors
    if(MSVC)
        target_compile_options(pntr_app_example_web PRIVATE /W4 /WX)
    else()
        target_compile_options(pntr_app_example_web PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endif()
