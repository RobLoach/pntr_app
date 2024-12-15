# this loads the helper
find_package(pntr REQUIRED)

# for this lib, use the local pntr_app dir
# most projects should not set this
# but we want to be able to test things from this repo, on the fly
set(PNTR_APP_LOCATION "${PROJECT_SOURCE_DIR}" CACHE INTERNAL "")

message("Building pntr_app examples using ${PROJECT_SOURCE_DIR}")


# in this repo, we want to test a lot of different backends
# but in your setup it can be much simpler (see README)
# you can also use --target to only build certain things

# basic pntr_app_example
add_executable(pntr_app_example pntr_app_example.c)
add_pntr(pntr_app_example)

# I want to put web output in /docs & example output in / and tune the emscritpen build a bit
if (EMSCRIPTEN)
  set_target_properties(pntr_app_example
    PROPERTIES
    SUFFIX ".mjs"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/docs"
    LINK_FLAGS "-s ASSERTIONS=1 -sSINGLE_FILE --preload-file ../assets@/assets"
  )
else()
    set_target_properties(pntr_app_example
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}"
  )
endif()

# force use of raylib
if ("${EXAMPLE}" STREQUAL "RAYLIB")
    add_executable(pntr_app_example-raylib pntr_app_example.c)
    add_pntr(pntr_app_example-raylib RAYLIB)
endif()

# build SDL2 example
if ("${EXAMPLE}" STREQUAL "SDL2")
    add_executable(pntr_app_example-sdl2 pntr_app_example.c)
    add_pntr(pntr_app_example-sdl2 SDL2)
endif()

# build libretro example core
if ("${EXAMPLE}" STREQUAL "RETRO")
    add_library(pntr_app_example-libretro SHARED pntr_app_example.c)
    add_pntr(pntr_app_example-libretro RETRO)
    set_target_properties(pntr_app_example-libretro PROPERTIES PREFIX "")
endif()

