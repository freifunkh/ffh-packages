cmake_minimum_required(VERSION 2.8.8...3.5.0)

project(FFHAUTHORIZEDFINGERPRINTS C)

set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS _GNU_SOURCE)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

find_path(UBOX_INCLUDE_DIR NAMES libubox/uloop.h)
find_library(UBOX_LIBRARY NAMES ubox)

find_package(PkgConfig REQUIRED QUIET)
pkg_check_modules(ECDSAUTIL REQUIRED ecdsautil)

find_package(json-c CONFIG REQUIRED)

include_directories(${UBOX_INCLUDE_DIR} ${ECDSAUTIL_INCLUDE_DIRS})

add_executable(ffh-authorized-fingerprints
  ffh-authorized-fingerprints.c
)

add_library(respondd SHARED respondd.c ffh-authorized-fingerprints.c)
set_property(TARGET respondd PROPERTY COMPILE_FLAGS "-std=c99 -Wall -Wextra -Wpedantic -Werror")
target_link_libraries(respondd
    m
    gluonutil
    json-c::json-c
    ${ECDSAUTIL_LIBRARIES}
    ${UBOX_LIBRARY}
)

set_property(TARGET ffh-authorized-fingerprints PROPERTY COMPILE_FLAGS "-std=c99 -Wall -Wextra -Wpedantic -Werror")
target_link_libraries(ffh-authorized-fingerprints
    m
    ${ECDSAUTIL_LIBRARIES}
    ${UBOX_LIBRARY}
)

install(TARGETS ffh-authorized-fingerprints RUNTIME DESTINATION sbin)
install(TARGETS respondd LIBRARY DESTINATION lib/respondd)

