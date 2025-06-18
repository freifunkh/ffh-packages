cmake_minimum_required(VERSION 2.8.8...3.5.0)

project(FFHWATCHDOG C)

set_property(DIRECTORY PROPERTY COMPILE_DEFINITIONS _GNU_SOURCE)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

find_path(UBOX_INCLUDE_DIR NAMES libubox/uloop.h)
find_library(UBOX_LIBRARY NAMES ubox)

FIND_PATH(UBUS_INCLUDE_DIR NAMES libubus.h)
FIND_LIBRARY(UBUS_LIBRARY NAMES ubus)

find_package(PkgConfig REQUIRED QUIET)

find_package(json-c CONFIG REQUIRED)

include_directories(${UBOX_INCLUDE_DIR} ${UBUS_INCLUDE_DIR})


add_library(watchdog
  SHARED
  watchdog.c
)

add_library(respondd SHARED respondd.c watchdog.c)

add_executable(ffh-watchdog
  ffh-watchdog.c
)


set_property(TARGET respondd PROPERTY COMPILE_FLAGS "-std=c99 -Wall -Wextra -Wpedantic -Werror")
target_link_libraries(respondd
    m
    json-c::json-c
    ${UBOX_LIBRARY}
    ${UBUS_LIBRARY}
)

set_property(TARGET watchdog PROPERTY COMPILE_FLAGS "-std=c99 -Wall -Wextra -Wpedantic -Werror")
target_link_libraries(watchdog
    m
    ubus
    ${UBOX_LIBRARY}
    ${UBUS_LIBRARY}
)

set_property(TARGET ffh-watchdog PROPERTY COMPILE_FLAGS "-std=c99 -Wall -Wextra -Wpedantic -Werror")
target_link_libraries(ffh-watchdog
    m
    ${UBOX_LIBRARY}
    ${UBUS_LIBRARY}
    watchdog
)


install(TARGETS watchdog LIBRARY DESTINATION lib)
install(TARGETS respondd LIBRARY DESTINATION lib/respondd)
install(TARGETS ffh-watchdog RUNTIME DESTINATION sbin)

