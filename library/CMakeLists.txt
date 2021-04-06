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
        src/config.cc
        src/draw.cc
        src/hacks/skin.cc
        src/hacks/misc.cc
        src/hacks/chams.cc
        src/hacks/grief.cc
        src/hacks/visuals.cc
        src/hacks/predict.cc
        src/hacks/chat.cc
        src/external/Yaml.cc
        src/ui/lib.cc
        src/ui/test.cc
)

if (MSVC)
else()
    target_compile_options(scareware PRIVATE "-fpermissive")
endif()


target_include_directories(scareware PUBLIC include)