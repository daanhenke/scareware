project(injector)

add_executable(scareinjector)

target_sources(scareinjector
    PRIVATE
        src/main.cc
        src/winutil.cc
        src/ManualMappedLibraryLoader.cc
)

if (MSVC)
    target_compile_options(scareinjector PRIVATE "/GS-")
else()
    target_compile_options(scareinjector PRIVATE "-fpermissive")
endif()

target_include_directories(scareinjector PUBLIC include)