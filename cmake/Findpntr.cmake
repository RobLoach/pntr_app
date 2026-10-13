# pntr
include(FetchContent)
FetchContent_Declare(
    pntr
    URL https://github.com/RobLoach/pntr/archive/refs/tags/v0.1.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(pntr)
