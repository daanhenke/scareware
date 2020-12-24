project(injector)

add_executable(scareinjector)

target_sources(scareinjector
    PRIVATE
        src/main.cc
        src/winutil.cc
        src/ManualMappedLibraryLoader.cc
)

set_target_properties(scareinjector PROPERTIES COMPILE_FLAGS "/Zc:wchar_t-")

target_include_directories(scareinjector PUBLIC include)