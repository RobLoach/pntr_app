# this loads the helper
find_package(pntr REQUIRED)

# create pntr_app_example
add_executable(pntr_app_example src/main.c)

# for this lib, use the local pntr_app repo
# most projects should not set this
set(PNTR_APP_LOCATION, "${PROJECT_SOURCE_DIR}")

# this will use EXAMPLES define to set the targets
# so we can try out different backends quickly
# in yours, you can leave off any options, and get basic default raylib
add_pntr(pntr_app_example ${EXAMPLES})
