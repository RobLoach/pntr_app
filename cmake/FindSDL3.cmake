include(FetchContent)
FetchContent_Declare(
    SDL3Source
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG a962f40bbba175e9716557a25d5d7965f134a3d3
    #GIT_SHALLOW 1
)
FetchContent_MakeAvailable(SDL3Source)
