cmake_minimum_required(VERSION 3.4.1)

add_library( # Sets the name of the library.
        openslaudio-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/openslaudio-lib.cpp)


target_link_libraries( # Specifies the target library.
        openslaudio-lib
        OpenSLES
        # Links the target library to the log library
        # included in the NDK.
        log)