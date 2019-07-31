cmake_minimum_required(VERSION 3.4.1)

add_library( # Sets the name of the library.
        music-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/music-lib.cpp)


target_link_libraries( # Specifies the target library.
        music-lib
       # OpenSLES
        # Links the target library to the log library
        # included in the NDK.
        log)