# SDL2_mixer
include(FetchContent)
FetchContent_Declare(
    SDL2MixerSource
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG release-2.6.3
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(SDL2MixerSource)
#include_directories(${sdl2mixersource_SOURCE_DIR}/include)
