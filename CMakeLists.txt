cmake_minimum_required(VERSION 3.13)
project(pntr_app
    DESCRIPTION "pntr_app: Manage a pntr application across a number of different platforms."
    HOMEPAGE_URL "https://github.com/robloach/pntr_app"
    VERSION 0.1.0
    LANGUAGES C
)

include(GNUInstallDirs)

# pntr_app
add_library(pntr_app INTERFACE)
target_include_directories(pntr_app INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Options
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(PNTR_APP_IS_MAIN TRUE)
else()
    set(PNTR_APP_IS_MAIN FALSE)
endif()

if (${PNTR_APP_IS_MAIN} AND EMSCRIPTEN)
    set(PNTR_APP_BUILD_EXAMPLE_WEB_DEFAULT TRUE)
else()
    set(PNTR_APP_BUILD_EXAMPLE_WEB_DEFAULT FALSE)
endif()

option(PNTR_APP_BUILD_EXAMPLE "Example" ${PNTR_APP_IS_MAIN})
option(PNTR_APP_BUILD_EXAMPLE_SDL "Example: SDL" ${PNTR_APP_IS_MAIN})
option(PNTR_APP_BUILD_EXAMPLE_RAYLIB "Example: raylib" ${PNTR_APP_IS_MAIN})
option(PNTR_APP_BUILD_EXAMPLE_CLI "Example: CLI" ${PNTR_APP_IS_MAIN})
option(PNTR_APP_BUILD_EXAMPLE_RETRO "Example: Libretro" ${PNTR_APP_IS_MAIN})
option(PNTR_APP_BUILD_EXAMPLE_WEB "Example: Emscripten" ${PNTR_APP_BUILD_EXAMPLE_WEB_DEFAULT})
option(PNTR_APP_BUILD_TEST "Example: Test" ${PNTR_APP_IS_MAIN})
option(PNTR_APP_INSTALL "Install" ${PNTR_APP_IS_MAIN})

if (PNTR_APP_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

if (PNTR_APP_BUILD_TEST AND NOT EMSCRIPTEN)
    # Testing
    include(CTest)
    enable_testing()
    if (BUILD_TESTING)
        # set(CTEST_CUSTOM_TESTS_IGNORE
        #     pkg-config--static
        # )
        # Always print verbose output when tests fail if run using `make test`.
        list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
        add_subdirectory(test)
    endif()
endif()

# Install
if (PNTR_APP_INSTALL)
    include(CMakePackageConfigHelpers)

    set(PNTR_APP_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/pntr_app")

    # Target
    install(TARGETS pntr_app
        EXPORT pntr_appTargets
    )

    # Headers
    install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(EXPORT pntr_appTargets
        FILE pntr_appTargets.cmake
        DESTINATION ${PNTR_APP_INSTALL_CMAKEDIR}
    )

    # Package configuration
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/pntr_appConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/pntr_appConfig.cmake"
        INSTALL_DESTINATION ${PNTR_APP_INSTALL_CMAKEDIR}
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/pntr_appConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/pntr_appConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/pntr_appConfigVersion.cmake"
        DESTINATION ${PNTR_APP_INSTALL_CMAKEDIR}
    )
endif()
