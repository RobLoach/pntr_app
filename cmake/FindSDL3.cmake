include(FetchContent)
FetchContent_Declare(
    SDL3Source
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG ac2edad8090a4bf79d0bd11a5860c7c10ad73fb2
    #GIT_SHALLOW 1
)
FetchContent_MakeAvailable(SDL3Source)
