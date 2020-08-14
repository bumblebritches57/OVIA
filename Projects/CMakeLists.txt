cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(OVIA C)

if(CMAKE_CONFIGURATION_TYPES MATCHES "Debug" OR CMAKE_CONFIGURATION_TYPES MATCHES "RelWithDebInfo")
    enable_testing()
endif(CMAKE_CONFIGURATION_TYPES MATCHES "Debug" OR CMAKE_CONFIGURATION_TYPES MATCHES "RelWithDebInfo")

if(NOT OVIA_VERSION)
    execute_process(COMMAND ${GIT_EXECUTABLE} git rev-parse --abbrev-ref HEAD WORKING_DIRECTORY CMAKE_CURRENT_SOURCE_DIR OUTPUT_VARIABLE GIT_BRANCH_NAME RESULT_VARIABLE STDOUT ERROR_VARIABLE STDOUT)

    execute_process(COMMAND ${GIT_EXECUTABLE} git rev-parse --verify ${GIT_BRANCH_NAME} WORKING_DIRECTORY CMAKE_CURRENT_SOURCE_DIR OUTPUT_VARIABLE GIT_COMMIT_ID RESULT_VARIABLE STDOUT ERROR_VARIABLE STDOUT)

    set(OVIA_VERSION_CommitID ${GIT_COMMIT_ID})
    set(OVIA_VERSION ${OVIA_VERSION_MAJOR}.${OVIA_VERSION_MINOR}.${OVIA_VERSION_CommitID})
endif(NOT OVIA_VERSION)

if(${CMAKE_SYSTEM_NAME} MATCHES "UNIX" OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-D_LARGEFILE_SOURCE)
    add_definitions(-D_LARGEFILE64_SOURCE)
    add_definitions(-D_FILE_OFFSET_BITS=64)
    add_definitions(-D__STDC_WANT_LIB_EXT1__)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
    add_definitions(-D_ATL_ALLOW_CHAR_UNSIGNED)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(${CMAKE_SYSTEM_NAME} MATCHES "UNIX" OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

set(Library_Dir "${CMAKE_CURRENT_SOURCE_DIR}/../Library")

set(Test_Dir "${CMAKE_CURRENT_SOURCE_DIR}/../Tests")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../BUILD/${ARCHITECTURE}/${BUILDTYPE}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../BUILD/${ARCHITECTURE}/${BUILDTYPE}")
set(RUNTIME_OUTPUT_DIRECTORY       "${CMAKE_CURRENT_SOURCE_DIR}/../BUILD/${ARCHITECTURE}/${BUILDTYPE}")

configure_file("${Library_Dir}/../Resources/OVIA.pc.in" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/OVIA.pc")

if(NOT ${CMAKE_BUILD_TYPE})
    set(CMAKE_BUILD_TYPE "${MinSizeRel}")
endif(NOT ${CMAKE_BUILD_TYPE})

option(BUILD_SHARED_LIBS "Should FoundationIO be compiled as a dynamic library (.dylib/.dll/.so)?" ON)

set(OVIA_PUBLICHEADERS
    "${Library_Dir}/include/OVIA.h"
)

set(Library_Sources
    "${Library_Dir}/src/OVIA.c"
    "${Library_Dir}/src/MediaIO.c"
    "${Library_Dir}/src/CodecIO/FLAC.c"
    "${Library_Dir}/src/CodecIO/JPEG.c"
    "${Library_Dir}/src/CodecIO/PNG.c"
    "${Library_Dir}/src/CodecIO/SamplesIO.c"
    "${Library_Dir}/src/EntropyIO/EntropyIO.c"
    "${Library_Dir}/src/EntropyIO/Arithmetic.c"
    "${Library_Dir}/src/EntropyIO/Flate.c"
    "${Library_Dir}/src/EntropyIO/Huffman.c"
    "${Library_Dir}/src/StreamIO/AIF.c"
    "${Library_Dir}/src/StreamIO/BMP.c"
    "${Library_Dir}/src/StreamIO/PNM.c"
    "${Library_Dir}/src/StreamIO/W64.c"
    "${Library_Dir}/src/StreamIO/RIFF.c"
    "${Library_Dir}/src/TagIO/TagIO.c"
    "${Library_Dir}/src/TagIO/AIFTags.c"
    "${Library_Dir}/src/TagIO/APETags.c"
    "${Library_Dir}/src/TagIO/ID3.c"
    "${Library_Dir}/src/TagIO/Vorbis.c"
    "${Library_Dir}/src/TagIO/WAVTags.c"
    "${Library_Dir}/src/TransformIO/RCT.c"
    "${Library_Dir}/src/TransformIO/YCoCgR.c"
)

if(${BUILD_SHARED_LIBS} MATCHES OFF)
    add_library(OVIA STATIC ${Library_Sources})
elseif(${BUILD_SHARED_LIBS} MATCHES ON)
    add_library(OVIA SHARED ${Library_Sources})
endif(${BUILD_SHARED_LIBS} MATCHES OFF)

target_include_directories(OVIA PUBLIC "${Library_Dir}/include")

if(NOT DEFINED ${CMAKE_INSTALL_PREFIX})
if(${CMAKE_SYSTEM_NAME} MATCHES "UNIX" OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_INSTALL_PREFIX "/usr/local")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if(${CMAKE_GENERATOR_PLATFORM} MATCHES x64)
        set(CMAKE_INSTALL_PREFIX "%ProgramFiles%\FoundationIO")
    elseif(${CMAKE_GENERATOR_PLATFORM} MATCHES x86)
        set(CMAKE_INSTALL_PREFIX "%ProgramFiles(x86)%\FoundationIO")
    endif(${CMAKE_GENERATOR_PLATFORM} MATCHES x64)
endif(${CMAKE_SYSTEM_NAME} MATCHES "UNIX" OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
endif(NOT DEFINED ${CMAKE_INSTALL_PREFIX})

install(TARGETS OVIA DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES ${OVIA_PUBLICHEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/OVIA.pc" DESTINATION "${CMAKE_INSTALL_PREFIX}/share/pkgconfig")

if(NOT ${CMAKE_BUILD_TYPE})
  set(CMAKE_BUILD_TYPE "${MinSizeRel}")
endif()

add_subdirectory(../Dependencies/FoundationIO/Projects "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/../Dependencies/FoundationIO/BUILD")

set_target_properties(OVIA PROPERTIES PREFIX "")

TARGET_LINK_LIBRARIES(OVIA FoundationIO)

if(${CMAKE_C_COMPILER_ID} MATCHES "AppleClang" OR ${CMAKE_C_COMPILER_ID} MATCHES "Clang")

set(CMAKE_C_FLAGS " \
${CMAKE_C_FLAGS} \
-fPIC \
-fshort-enums \
-funsigned-char \
-std=c18 \
-Weverything \
-Wall \
-Wextra \
-Wpedantic \
-Wno-assign-enum \
-Wno-c99-compat \
-Wno-ignored-attributes \
-Wno-incompatible-pointer-types-discards-qualifiers \
-Wno-padded \
-Wno-pointer-sign \
-Wno-reserved-id-macro \
-Wno-sign-conversion \
-Wsuggest-attribute=const \
-Wsuggest-attribute=pure \
-Wsuggest-attribute=malloc \
-Wsuggest-attribute=noreturn \
-Wsuggest-attribute=format \
-Wmissing-format-attribute \
-Wmissing-noreturn \
-Wenum-conversion \
-v \
")

set(CMAKE_C_FLAGS_DEBUG " \
${CMAKE_C_FLAGS} \
-fsanitize=address,undefined \
-fno-omit-frame-pointer \
-DDEBUG=1 \
-g \
-O1 \
")

set(CMAKE_C_FLAGS_RELEASE " \
${CMAKE_C_FLAGS} \
-Ofast \
")

set(CMAKE_C_FLAGS_RELWITHDEBINFO " \
${CMAKE_C_FLAGS} \
-fsanitize=address,undefined \
-fno-omit-frame-pointer \
-DDEBUG=1 \
-g \
-Ofast \
")

set(CMAKE_C_FLAGS_MINSIZEREL " \
${CMAKE_C_FLAGS} \
-Oz \
")

set(LINK_FLAGS ${LINK_FLAGS} " \
-flto=full \
-lm \
")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(LINK_FLAGS ${LINK_FLAGS} " \
-dead_strip_dylibs
")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

set(LINK_FLAGS_DEBUG " \
${LINK_FLAGS} \
")

set(LINK_FLAGS_RELWITHDEBINFO " \
${LINK_FLAGS} \
")

set(LINK_FLAGS_RELEASE " \
${LINK_FLAGS} \
")

set(LINK_FLAGS_MINSIZEREL " \
${LINK_FLAGS} \
")

elseif(${CMAKE_C_COMPILER_ID} MATCHES "GNU")

set(CMAKE_C_FLAGS " \
${CMAKE_C_FLAGS} \
-fPIC \
-fshort-enums \
-funroll-loops \
-funsigned-char \
-std=c2x \
-Wall \
-Wextra \
-Wpedantic \
-Wno-ignored-attributes \
-Wno-padded \
-Wno-pointer-sign \
-Wno-sign-conversion \
-Wno-varargs \
-Wsuggest-attribute=const \
-Wsuggest-attribute=pure \
-Wsuggest-attribute=malloc \
-Wsuggest-attribute=noreturn \
-Wsuggest-attribute=format \
-Wmissing-format-attribute \
-Wmissing-noreturn \
")

set(CMAKE_C_FLAGS_DEBUG " \
${CMAKE_C_FLAGS} \
-fsanitize=address,undefined \
-DDEBUG=1 \
-g \
-O1 \
")

set(CMAKE_C_FLAGS_RELEASE " \
${CMAKE_C_FLAGS} \
-Ofast \
")

set(CMAKE_C_FLAGS_RELWITHDEBINFO " \
${CMAKE_C_FLAGS} \
-fsanitize=address,undefined \
-fno-omit-frame-pointer \
-DDEBUG=1 \
-g \
-Ofast \
")

set(CMAKE_C_FLAGS_MINSIZEREL " \
${CMAKE_C_FLAGS} \
-Os \
")

set(LINK_FLAGS ${LINK_FLAGS} " \
-flto=full \
-lm \
")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
set(LINK_FLAGS ${LINK_FLAGS} " \
-dead_strip_dylibs
")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

set(LINK_FLAGS_DEBUG " \
${LINK_FLAGS} \
")

set(LINK_FLAGS_RELWITHDEBINFO " \
${LINK_FLAGS} \
")

set(LINK_FLAGS_RELEASE " \
${LINK_FLAGS} \
")

set(LINK_FLAGS_MINSIZEREL " \
${LINK_FLAGS} \
")

elseif(${MAKE_C_COMPILER_ID} MATCHES "MSVC")

set(CMAKE_C_FLAGS " \
${CMAKE_C_FLAGS} \
/bigobj \
/FAu \
/GF \
/Gm- \
/GT \
/Gy \
/JMC \
/J \
/MP \
/MT \
/Ob2 \
/openmp \
/p:CharacterSet=Unicode \
/p:FunctionLevelLinking=true \
/p:PrecompiledHeader=NotUsing \
/p:RuntimeLibrary=MultiThreaded \
/p:UseOfMfc=false \
/p:WarningLevel=Level3 \
/Qpar \
/sdl- \
/std:c++17 \
/W3 \
/Zc:rvalueCast \
/Zc:wchar_t \
/Wall \
")

set(CMAKE_C_FLAGS_DEBUG " \
${CMAKE_C_FLAGS} \
/DDEBUG=1 \
/D_DEBUG=1 \
/p:UseDebugLibraries=true \
/O1 \
")

set(CMAKE_C_FLAGS_RELEASE " \
${CMAKE_C_FLAGS} \
/p:UseDebugLibraries=false \
/Ox \
")

set(CMAKE_C_FLAGS_RELWITHDEBINFO " \
${CMAKE_C_FLAGS} \
/DDEBUG=1 \
/D_DEBUG=1 \
/p:UseDebugLibraries=true \
-Ox \
")

set(CMAKE_C_FLAGS_MINSIZEREL " \
${CMAKE_C_FLAGS} \
/p:UseDebugLibraries=false \
-Os \
")

set(LINK_FLAGS " \
${LINK_FLAGS} \
/GL \
/LTCG:INCREMENTAL
/OPT:REF
/flto \
")

set(LINK_FLAGS_DEBUG " \
${LINK_FLAGS} \
/DEBUG \
")


set(LINK_FLAGS_RELWITHDEBINFO " \
${LINK_FLAGS} \
/DEBUG \
/RELEASE \
")

set(LINK_FLAGS_RELEASE " \
${LINK_FLAGS} \
/RELEASE \
")

set(LINK_FLAGS_MINSIZEREL " \
${LINK_FLAGS} \
/RELEASE \
")

endif(${CMAKE_C_COMPILER_ID} MATCHES "AppleClang" OR ${CMAKE_C_COMPILER_ID} MATCHES "Clang")
