include(FetchContent)
FetchContent_Declare(
    SDL3_mixerSource
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG cb0882353f8e7e4da00a36cab16c223efdbe71d6
    #GIT_SHALLOW 1
)
FetchContent_MakeAvailable(SDL3_mixerSource)
