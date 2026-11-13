# RGFW
include(FetchContent)
FetchContent_Declare(
    rgfw
    GIT_REPOSITORY https://github.com/ColleagueRiley/RGFW.git
    GIT_TAG c8e7ebcd38b81e4942d9fd702b74b5434af1a6c0
)
FetchContent_MakeAvailable(rgfw)
include_directories(${rgfw_SOURCE_DIR})
