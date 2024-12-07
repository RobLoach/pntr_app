# RSGL
include(FetchContent)
FetchContent_Declare(
    rsgl
    GIT_REPOSITORY https://github.com/ColleagueRiley/RSGL.git
    GIT_TAG ee36b38
)
FetchContent_MakeAvailable(rsgl)
include_directories(${rsgl_SOURCE_DIR})