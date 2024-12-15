# SDL2
include(FetchContent)
FetchContent_Declare(
    SDL2Source
    URL https://github.com/libsdl-org/SDL/releases/download/release-2.30.10/SDL2-2.30.10.zip
)
FetchContent_MakeAvailable(SDL2Source)
#include_directories(${SDL2Source_SOURCE_DIR})
