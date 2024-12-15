cmake_minimum_required(VERSION 3.13)
project(pntr_app
    DESCRIPTION "pntr_app: Manage a pntr application across a number of different platforms."
    HOMEPAGE_URL "https://github.com/robloach/pntr_app"
    VERSION 0.0.1
    LANGUAGES C
)

option(PNTR_APP_BUILD_TEST "Build Unit-tests" TRUE)
option(PNTR_APP_BUILD_DEMOS "Build Examples" TRUE)

# find cmake stuff in current dir/cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# general settings that make things build better

# https://cmake.org/cmake/help/latest/policy/CMP0135.html
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
  set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
endif()

# https://cmake.org/cmake/help/latest/policy/CMP0077.html
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
  set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
endif()

# enable colors for ninja
set(CMAKE_COLOR_DIAGNOSTICS ON)

# everyone wants to make an "uninstall" target
set_property(GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS ON)

# switch these for debugging
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

if (EMSCRIPTEN)
    option(EXAMPLE "For pntr_app: build all the examples with this sound/window system" "EMSCRIPTEN")
else()
    option(EXAMPLE "For pntr_app: build all the examples with this sound/window system" "RAYLIB")
endif()

# sub-projects

if (PNTR_APP_BUILD_TEST)
    add_subdirectory(test)
endif()

if (PNTR_APP_BUILD_DEMOS)
    add_subdirectory(example)
endif()
