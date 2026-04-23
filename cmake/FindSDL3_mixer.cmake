include(FetchContent)
FetchContent_Declare(
    SDL3_mixerSource
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG release-3.2.0
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(SDL3_mixerSource)
