# RGFW
include(FetchContent)
FetchContent_Declare(
    rgfw
    GIT_REPOSITORY https://github.com/ColleagueRiley/RGFW.git
    GIT_TAG 1.8.1
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(rgfw)
include_directories(${rgfw_SOURCE_DIR})
