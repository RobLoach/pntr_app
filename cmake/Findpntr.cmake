# setup different permutations of pntr/pntr_app and dependencies
# this file should be all a user needs to add pntr_app to their project

# TODO: use PNTR_APP_LOCATION instead of USE_LOCAL_PNTR_APP

# TODO: this needs a bit of updating to work like docs
# TODO: function should just pull deps if PNTR_APP_WINDOW_* and PNTR_APP_SOUND_* are already set

## usage example
## order does not matter, and it's case-insensitive.
# add_executable(mything main.c)
# add_pntr(mything TERMBOX RAYLIB_SOUND)

## available window options:
# NO_WINDOW
# RAYLIB
# RAYLIB_OLDPI
# RAYLIB_DRM
# SDL
# RETRO
# TERMBOX
# CLI

## available sound options
# NO_SOUND
# RAYLIB_SOUND
# SDL_SOUND

## DEFAULTS to RAYLIB/RAYLIB_SOUND or EMSCRIPTEN (if running in emcmake)

# this contains the main-override and basic window/input setup, like this is the "frame" of things
set(PNTR_APP_DEFAULT_WINDOW "RAYLIB")

# this contains the sound lib in most cases, this is tied to window lib, but sometimes not (like CLI)
set(PNTR_APP_DEFAULT_SOUND "RAYLIB")

# global-option to allow in-tree pntr_app (for demos in that repo)
option(USE_LOCAL_PNTR_APP "Force using the current directory as source of pntr_app" OFF)

include(FetchContent)

# this causes crt error on mac, but you might want it for other things
# SET (CMAKE_EXE_LINKER_FLAGS "-static")

# you might need this if you are building sdl/raylib
# IF(APPLE)
#     set(CMAKE_THREAD_LIBS_INIT "-lpthread")
#     set(CMAKE_HAVE_THREADS_LIBRARY 1)
#     set(CMAKE_USE_WIN32_THREADS_INIT 0)
#     set(CMAKE_USE_PTHREADS_INIT 1)
#     set(THREADS_PREFER_PTHREAD_FLAG ON)
# ENDIF()

# this will allow you to easily add pntr/pntr_app to your application
function(add_pntr target)
  # pull out all the arguments and turn it into PNTR_APP_WINDOW/PNTR_APP_SOUND
  if (${ARGC} GREATER 1)
    list(SUBLIST ARGV 1 -1 pntr_args)
    string(TOUPPER "${pntr_args}" pntr_args)
    list(JOIN pntr_args ", " display_args)
    foreach(a IN LISTS pntr_args)
      if ("${a}" STREQUAL "RAYLIB")
        set(PNTR_APP_WINDOW "RAYLIB")
      elseif ("${a}" STREQUAL "RAYLIB_SOUND")
        set(PNTR_APP_SOUND "RAYLIB")
      elseif ("${a}" STREQUAL "RAYLIB_OLDPI")
        set(PNTR_APP_WINDOW "RAYLIB")
        # TODO: add define for raylib build
      elseif ("${a}" STREQUAL "RAYLIB_DRM")
        set(PNTR_APP_WINDOW "RAYLIB")
        # TODO: add define for raylib build
      elseif ("${a}" STREQUAL "SDL")
        set(PNTR_APP_WINDOW "SDL")
      elseif ("${a}" STREQUAL "SDL_SOUND")
        set(PNTR_APP_SOUND "SDL")
      elseif ("${a}" STREQUAL "RETRO")
        set(PNTR_APP_WINDOW "RETRO")
        set(PNTR_APP_SOUND "RETRO")
      elseif ("${a}" STREQUAL "CLI")
        set(PNTR_APP_WINDOW "CLI")
      elseif ("${a}" STREQUAL "TERMBOX")
        set(PNTR_APP_WINDOW "TERMBOX")
      elseif ("${a}" STREQUAL "NO_SOUND")
        set(PNTR_APP_SOUND "NONE")
      elseif ("${a}" STREQUAL "NO_WINDOW")
        set(PNTR_APP_WINDOW "NONE")
      endif()
    endforeach()
  else()
    set(display_args "DEFAULTS")
  endif()
  message("-- pntr_app added to ${target}: ${display_args}")

  # check for sensible defaults, like if you set SDL option, but not PNTR_APP_SOUND (it should be SDL_SOUND)
  if(NOT DEFINED PNTR_APP_SOUND)
    if ("${PNTR_APP_WINDOW}" STREQUAL "RAYLIB")
      set(PNTR_APP_SOUND "RAYLIB")
    endif()
    if ("${PNTR_APP_WINDOW}" STREQUAL "SDL")
      set(PNTR_APP_SOUND "SDL")
    endif()
    if ("${PNTR_APP_WINDOW}" STREQUAL "TERMBOX")
      set(PNTR_APP_SOUND "NONE")
    endif()
    if ("${PNTR_APP_WINDOW}" STREQUAL "CLI")
      set(PNTR_APP_SOUND "NONE")
    endif()
  endif()

  if(NOT DEFINED PNTR_APP_WINDOW)
    if ("${PNTR_APP_SOUND}" STREQUAL "RAYLIB")
      set(PNTR_APP_WINDOW "RAYLIB")
    endif()
    if ("${PNTR_APP_SOUND}" STREQUAL "SDL")
      set(PNTR_APP_WINDOW "SDL")
    endif()
  endif()

  # set defaults
  if (NOT DEFINED PNTR_APP_WINDOW)
    if (EMSCRIPTEN)
      set(PNTR_APP_WINDOW "EMSCRIPTEN")
    else()
      set(PNTR_APP_WINDOW "${PNTR_APP_DEFAULT_WINDOW}")
    endif()
  endif()
  if (NOT DEFINED PNTR_APP_SOUND)
    if (EMSCRIPTEN)
      set(PNTR_APP_SOUND "EMSCRIPTEN")
    else()
      set(PNTR_APP_SOUND "${PNTR_APP_DEFAULT_SOUND}")
    endif()
  endif()

  # TODO: here I could check warn about unsupported/bad combos

  # create a normalized library-name
  set(pntr_lib_name "pntr_app_${PNTR_APP_WINDOW}_${PNTR_APP_SOUND}")
  string(TOLOWER "${pntr_lib_name}" pntr_lib_name)
  string(REPLACE "raylib_raylib" "raylib" pntr_lib_name "${pntr_lib_name}")
  string(REPLACE "sdl_sdl" "sdl" pntr_lib_name "${pntr_lib_name}")
  string(REPLACE "retro_retro" "retro" pntr_lib_name "${pntr_lib_name}")
  string(REPLACE "emscripten_emscripten" "emscripten" pntr_lib_name "${pntr_lib_name}")

  # TODO: add suffix for different options, like pntr_app_raylib_drm_raylib

  message("--   PNTR_APP_WINDOW: ${PNTR_APP_WINDOW}")
  message("--   PNTR_APP_SOUND: ${PNTR_APP_SOUND}")
  message("--   Lib Name: ${pntr_lib_name}")
  message("--   Downloading/building dependencies.")

  # download deps that are needed by window/sound (that have not been downloaded)

  FetchContent_Declare(fpntr
    URL "https://github.com/robloach/pntr/archive/refs/heads/master.zip"
  )
  FetchContent_MakeAvailable(fpntr)

  if ($USE_LOCAL_PNTR_APP)
    set(fpntr_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}")
  else()
    FetchContent_Declare(fpntr_app
      URL "https://github.com/robloach/pntr_app/archive/refs/heads/master.zip"
    )
    FetchContent_MakeAvailable(fpntr_app)
  endif()

  # create a static library for all of pntr_app/pntr
  # here is where you link anything to lib you need
  # add_library(${pntr_lib_name} STATIC "${fpntr_SOURCE_DIR}/pntr_app.c")
  if (NOT TARGET "${pntr_lib_name}")
    add_library("${pntr_lib_name}" STATIC "${PROJECT_SOURCE_DIR}/tools/pntr_app.c")

    set_property(TARGET "${pntr_lib_name}" PROPERTY C_STANDARD 11)

    if ("${PNTR_APP_WINDOW}" STREQUAL "SDL")
      target_compile_definitions("${pntr_lib_name}" PUBLIC PNTR_APP_SDL)
    endif()

    if ("${PNTR_APP_SOUND}" STREQUAL "SDL")
      target_compile_definitions("${pntr_lib_name}" PUBLIC PNTR_APP_SDL_MIXER)
    endif()

    if ("${PNTR_APP_WINDOW}" STREQUAL "RAYLIB")
      target_compile_definitions("${pntr_lib_name}" PUBLIC PNTR_APP_RAYLIB)
    endif()

    # TODO: this isn't used yet, as RAYLIB sound/window are currently coupled, but they should be seperated
    if ("${PNTR_APP_SOUND}" STREQUAL "RAYLIB")
      target_compile_definitions("${pntr_lib_name}" PUBLIC PNTR_APP_RAYLIB_SOUND)
    endif()

    if ("${PNTR_APP_WINDOW}" STREQUAL "TERMBOX")
      target_compile_definitions("${pntr_lib_name}" PUBLIC PNTR_APP_CLI)
    endif()

    if ("${PNTR_APP_WINDOW}" STREQUAL "CLI")
      target_compile_definitions("${pntr_lib_name}" PUBLIC PNTR_APP_CLI)
      target_compile_definitions("${pntr_lib_name}" PUBLIC PNTR_APP_DISABLE_TERMBOX)
    endif()

    if ("${PNTR_APP_WINDOW}" STREQUAL "RETRO")
      target_compile_definitions("${pntr_lib_name}" PUBLIC PNTR_APP_LIBRETRO)
      set_target_properties("${pntr_lib_name}" PROPERTIES POSITION_INDEPENDENT_CODE ON)
    endif()

    if ("${PNTR_APP_WINDOW}" STREQUAL "EMSCRIPTEN")
      target_compile_definitions("${pntr_lib_name}" PUBLIC PNTR_APP_WEB)
    endif()

    # Strict Warnings and Errors
    # this fails on emscripten and libretro
    # if(MSVC)
    #     target_compile_options("${pntr_lib_name}" PRIVATE /W4 /WX)
    # else()
    #     target_compile_options("${pntr_lib_name}" PRIVATE -Wall -Wextra -Wpedantic -Werror)
    # endif()
  endif()

  target_link_libraries("${target}" "${pntr_lib_name}")
  include_directories("${fpntr_app_SOURCE_DIR}/include")
  include_directories("${fpntr_SOURCE_DIR}")

  if ("${PNTR_APP_SOUND}" STREQUAL "RETRO" OR "${PNTR_APP_WINDOW}" STREQUAL "RETRO")
    FetchContent_Declare(
      libretro
      URL "https://github.com/libretro/libretro-common/archive/refs/heads/master.zip"
    )
    FetchContent_MakeAvailable(libretro)
    include_directories("${libretro_SOURCE_DIR}/include")
    target_sources("${pntr_lib_name}" PRIVATE
      "${libretro_SOURCE_DIR}/audio/audio_mixer.c"
      "${libretro_SOURCE_DIR}/audio/conversion/float_to_s16.c"
      "${libretro_SOURCE_DIR}/memmap/memalign.c"
    )
  endif()

  if ("${PNTR_APP_WINDOW}" STREQUAL "RAYLIB")
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
    # TODO: handle options like DRM, etc
    FetchContent_Declare(
      raylib
      URL "https://github.com/raysan5/raylib/archive/refs/tags/5.5.zip"
    )
    FetchContent_MakeAvailable(raylib)
    target_link_libraries("${pntr_lib_name}" raylib)
    if (APPLE AND NOT EMSCRIPTEN)
      target_link_libraries("${pntr_lib_name}" "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
    endif()
  else()
    # raudio is uses for PNTR_APP_SOUND=RAYLIB to decouple them
    if ("${PNTR_APP_SOUND}" STREQUAL "RAYLIB" )
      FetchContent_Declare(
        raudio
        URL "https://github.com/raysan5/raudio/archive/refs/heads/master.zip"
      )
      FetchContent_MakeAvailable(raudio)
      target_link_libraries("${pntr_lib_name}" raudio)
      include_directories("${raudio_SOURCE_DIR}/src" "${raudio_SOURCE_DIR}/src/external")
    endif()
  endif()

  # SDL_MIXER needs SDL, so these are tied together
  if ("${PNTR_APP_SOUND}" STREQUAL "SDL" OR "${PNTR_APP_WINDOW}" STREQUAL "SDL")
    set(SDL_SHARED FALSE)
    set(SDL2_SHARED FALSE)
    set(SDL_STATIC TRUE)
    set(SDL_TEST FALSE)
    set(SDL_TESTS FALSE)
    set(INTERFACE_SDL2_SHARED FALSE)
    set(SDL2_DISABLE_UNINSTALL TRUE)
    set(SDL2_DISABLE_INSTALL TRUE)
    set(SDL_INSTALL_TESTS FALSE)

    FetchContent_Declare(
      SDL2
      URL "https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.30.10.zip"
    )
    FetchContent_MakeAvailable(SDL2)

    target_link_libraries("${pntr_lib_name}" SDL2::SDL2-static)
    include_directories(${SDL2_INCLUDE_DIRS})

    if ("${PNTR_APP_SOUND}" STREQUAL "SDL")
      FetchContent_Declare(
          SDL2_mixer
          URL "https://github.com/libsdl-org/SDL_mixer/archive/refs/tags/release-2.8.0.zip"
      )
      FetchContent_MakeAvailable(SDL2_mixer)

      set(SDL2MIXER_VORBIS STB)
      set(SDL2MIXER_INSTALL OFF)
      set(SDL2MIXER_DEPS_SHARED OFF)
      set(SDL2MIXER_VENDORED OFF)
      set(SDL2MIXER_SAMPLES OFF)
      set(SDL2MIXER_CMD OFF)
      set(SDL2MIXER_FLAC OFF)
      set(SDL2MIXER_MOD OFF)
      set(SDL2MIXER_MP3 OFF)
      set(SDL2MIXER_MIDI_NATIVE OFF)
      set(SDL2MIXER_OPUS OFF)
      set(SDL2MIXER_OPUS_SHARED OFF)
      set(SDL2MIXER_MIDI_FLUIDSYNTH OFF)
      set(SDL2MIXER_MP3_MPG123 OFF)
      set(SDL2MIXER_MP3_DRMP3 OFF)
      set(SDL2MIXER_MOD_XMP OFF)
      set(SDL2MIXER_MOD_MODPLUG OFF)
      set(SDL2MIXER_FLAC_DRFLAC OFF)
      set(SDL2MIXER_FLAC_LIBFLAC OFF)
      set(SDL2MIXER_VORBIS_VORBISFILE OFF)
      set(SDL2MIXER_WAVPACK OFF)
      set(SDL2MIXER_SAMPLES_INSTALL OFF)
      set(SDL2MIXER_BUILD_SHARED_LIBS OFF)

      target_link_libraries("${pntr_lib_name}" SDL2_mixer::SDL2_mixer-static)

      # this appears to be needed by sdl_mixer, at least on mac
      target_link_libraries("${pntr_lib_name}" GME)

      include_directories(${SDL2_mixer_INCLUDE_DIRS})
    endif()
  endif()
endfunction()
