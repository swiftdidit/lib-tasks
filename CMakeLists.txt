cmake_minimum_required(VERSION 3.29.3)
project(libtasks)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files for libtasks and liblist
set(LIBTASKS_SOURCES
    ${PROJECT_SOURCE_DIR}/src/libtasks.c
)

set(LIBLIST_SOURCES
    ${PROJECT_SOURCE_DIR}/src/liblist.c
)

# Create static libraries for libtasks and liblist
add_library(tasks STATIC ${LIBTASKS_SOURCES})
add_library(list STATIC ${LIBLIST_SOURCES})

# Specify the include directories for the libraries
target_include_directories(tasks PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(list PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Add the executable
add_executable(libtasks ${PROJECT_SOURCE_DIR}/src/main.c)

# Link the libraries to the executable
target_link_libraries(libtasks tasks list)