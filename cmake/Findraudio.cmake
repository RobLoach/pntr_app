# raudio
include(FetchContent)
FetchContent_Declare(
    raudio
    GIT_REPOSITORY https://github.com/raysan5/raudio.git
    GIT_TAG 8bab73b6ba8c15c8cf6cbbce72ef5d6e8735515d
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(raudio)
include_directories(${raudio_SOURCE_DIR}/src)
