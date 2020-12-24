project(library)

add_library(scareware SHARED)

target_sources(scareware
    PRIVATE
        src/main.cc
)

target_include_directories(scareware PUBLIC include)