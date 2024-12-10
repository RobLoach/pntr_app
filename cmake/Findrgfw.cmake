# RGFW
include(FetchContent)
FetchContent_Declare(
    rgfw
    GIT_REPOSITORY https://github.com/ColleagueRiley/RGFW.git
    GIT_TAG 1.06
    GIT_SHALLOW 1
)
FetchContent_MakeAvailable(rgfw)
include_directories(${rgfw_SOURCE_DIR})