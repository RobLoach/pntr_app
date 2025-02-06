include(FetchContent)
FetchContent_Declare(
    SDL3_mixerSource
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG ebdd9cc0fe43352e33ec234f4720fd7d54a31d13
    #GIT_SHALLOW 1
)
FetchContent_MakeAvailable(SDL3_mixerSource)
