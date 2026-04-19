# pntr
include(FetchContent)
FetchContent_Declare(
    pntr
    URL https://github.com/RobLoach/pntr/archive/refs/heads/master.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(pntr)
