cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(libOVIA)

if   (${CMAKE_SYSTEM_NAME} MATCHES "UNIX")
    set(libMATH -lm)
    add_definitions(-D_LARGEFILE_SOURCE)
    add_definitions(-D_LARGEFILE64_SOURCE)
    add_definitions(-D_FILE_OFFSET_BITS=64)
else (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-D_LIB)
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
    add_definitions(-D_ATL_ALLOW_CHAR_UNSIGNED)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(${CMAKE_SYSTEM_NAME} MATCHES "UNIX")

set(OVIA_Dir "${CMAKE_CURRENT_SOURCE_DIR}/../libOVIA")

set(OVIA_Headers
	"${OVIA_Dir}/include/libOVIA.h"
)

set(OVIA_Sources
    "${OVIA_Dir}/src/libOVIA.c"
    "${OVIA_Dir}/src/Decode/Audio/AIFDecoder.c"
    "${OVIA_Dir}/src/Decode/Audio/FLACDecoder.c"
    "${OVIA_Dir}/src/Decode/Audio/W64Decoder.c"
    "${OVIA_Dir}/src/Decode/Audio/WAVDecoder.c"
    "${OVIA_Dir}/src/Decode/Image/BMPDecoder.c"
    "${OVIA_Dir}/src/Decode/Image/PNGDecoder.c"
    "${OVIA_Dir}/src/Decode/Image/PNMDecoder.c"
    "${OVIA_Dir}/src/Encode/Audio/AIFEncoder.c"
    "${OVIA_Dir}/src/Encode/Audio/FLACEncoder.c"
    "${OVIA_Dir}/src/Encode/Audio/W64Encoder.c"
    "${OVIA_Dir}/src/Encode/Audio/WAVEncoder.c"
    "${OVIA_Dir}/src/Encode/Image/BMPEncoder.c"
    "${OVIA_Dir}/src/Encode/Image/PNGEncoder.c"
    "${OVIA_Dir}/src/Encode/Image/PNMEncoder.c"
)

add_library(${project} [static|shared] ${OVIA_Sources} ${OVIA_Headers})

if (CMAKE_C_COMPILER_ID MATCHES "AppleClang" OR CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(LINK_FLAGS " \
    ${LINK_FLAGS} \
    /flto \
    ")

    set(CMAKE_C_FLAGS " \
    ${CMAKE_C_FLAGS} \
    -fcolor-diagnostics \
    -ferror-limit=1024 \
    -fshort-enums \
    -fstrict-enums \
    -funroll-loops \
    -funsigned-char \
    -std=c11 \
    -Weverything \
    -Wno-c99-compat \
    -Wno-pointer-sign \
    -Wno-padded \
    ")

    set(CMAKE_C_FLAGS_DEBUG " \
    ${CMAKE_C_FLAGS} \
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
    -DDEBUG=1 \
    -g \
    -Ofast \
    ")

    set(CMAKE_C_FLAGS_MINSIZEREL " \
    ${CMAKE_C_FLAGS} \
    -Os \
    ")

elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
    set(LINK_FLAGS " \
    ${LINK_FLAGS} \
    /GL \
    ")

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
    /LTCG \
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
endif(CMAKE_C_COMPILER_ID MATCHES "AppleClang" OR CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID MATCHES "GNU")
