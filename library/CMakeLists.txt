project(library)

add_library(scareware SHARED)

target_sources(scareware
    PRIVATE
        src/main.cc
        src/memory.cc
        src/interfaces.cc
        src/console.cc
        src/VTableHook.cc
        src/hooks.cc
        src/logic.cc
        src/util.cc
        src/netvars.cc
        #src/events.cc
        src/hacks/skin.cc
        src/hacks/misc.cc
        src/hacks/chams.cc
)

target_include_directories(scareware PUBLIC include)