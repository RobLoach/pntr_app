FetchContent_Declare(
  retro
  URL "https://github.com/libretro/libretro-common/archive/refs/heads/master.zip"
)
FetchContent_MakeAvailable(retro)
include_directories("${retro_SOURCE_DIR}/include")
