project(injector)

add_executable(scareinjector)

target_sources(scareinjector
    PRIVATE
        src/main.cc
        src/winutil.cc
        src/ManualMappedLibraryLoader.cc
)

target_compile_options(scareinjector PRIVATE "/GS-")

target_include_directories(scareinjector PUBLIC include)