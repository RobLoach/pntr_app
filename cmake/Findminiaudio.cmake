# miniaudio
include(FetchContent)
FetchContent_Declare(
    miniaudio
    GIT_REPOSITORY https://github.com/mackron/miniaudio.git
    GIT_TAG 0.11.18
    GIT_SHALLOW 1
)

include_directories(${miniaudio_SOURCE_DIR})
