cmake_minimum_required(VERSION 3.11)
project(pntr_app
    DESCRIPTION "pntr_app: Manage a pntr application across a number of different platforms."
    HOMEPAGE_URL "https://github.com/robloach/pntr_app"
    VERSION 0.0.1
    LANGUAGES C
)

# pntr_app
add_library(pntr_app INTERFACE)
target_include_directories(pntr_app INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Options
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(PNTR_APP_IS_MAIN TRUE)
else()
    set(PNTR_APP_IS_MAIN FALSE)
endif()
option(PNTR_APP_BUILD_EXAMPLES "Build Examples" ${PNTR_APP_IS_MAIN})

if (PNTR_APP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
