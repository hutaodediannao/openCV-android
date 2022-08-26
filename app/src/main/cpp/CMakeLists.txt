# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.
project("myopencvndkapp")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../nativeLibs/${CMAKE_ANDROID_ARCH_ABI}")

include_directories(${CMAKE_SOURCE_DIR}/include)

set(openLibPath ${CMAKE_SOURCE_DIR})
#生成动态库openvc-lib
add_library(openvc-lib SHARED IMPORTED)
#设置输出的名称,设置动态库的版本和API版本
set_target_properties(openvc-lib
        PROPERTIES
        IMPORTED_LOCATION
        ${openLibPath}/nativeLibs/${ANDROID_ABI}/libopencv_java4.so)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

file(GLOB native_srcs ${CMAKE_SOURCE_DIR}/img-lib.cpp ${CMAKE_SOURCE_DIR}/native-lib.cpp)
add_library( # Sets the name of the library.
        myopencvndkapp

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${native_srcs})

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        myopencvndkapp
        android
        openvc-lib
        jnigraphics
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})