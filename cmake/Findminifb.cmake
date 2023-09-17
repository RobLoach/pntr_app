# minifb
include(FetchContent)
FetchContent_Declare(
    minifb
    GIT_REPOSITORY https://github.com/emoon/minifb.git
    GIT_TAG 37776c0e947f55345c7777ab1925b1a751abb9ee
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(minifb)
#include_directories(${SDL2Source_SOURCE_DIR})
