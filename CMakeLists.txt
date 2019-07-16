cmake_minimum_required(VERSION 2.8)

project(live-shaders)
add_executable(live-shaders actions/log.c actions/gtk_log.c actions/display.c actions/file.c events/main.c ui/main.c main.c)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
find_package(LibEpoxy REQUIRED)


# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries

include_directories(${GTK3_INCLUDE_DIRS} ${LIBEPOXY_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS} ${LIBEPOXY_LIBRARY_DIRS})

# Add other flags to the compiler

add_definitions(${GTK3_CFLAGS_OTHER} ${LIBEPOXY_CFLAG_OTHER})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

# Link the target to the GTK+ libraries
target_link_libraries(live-shaders ${GTK3_LIBRARIES} ${LIBEPOXY_LIBRARIES})

add_custom_command(TARGET live-shaders PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/shaders/ $<TARGET_FILE_DIR:live-shaders>/shaders/)
