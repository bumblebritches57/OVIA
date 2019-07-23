cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(OVIA C)

if (${CMAKE_CONFIGURATION_TYPES} MATCHES "Debug" OR ${CMAKE_CONFIGURATION_TYPES} MATCHES "RelWithDebInfo")
enable_testing()
endif (${CMAKE_CONFIGURATION_TYPES} MATCHES "Debug" OR ${CMAKE_CONFIGURATION_TYPES} MATCHES "RelWithDebInfo")

if   (${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR ${CMAKE_SYSTEM_NAME} MATCHES "UNIX")
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
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR ${CMAKE_SYSTEM_NAME} MATCHES "UNIX")

set(OVIA_Dir "${CMAKE_CURRENT_SOURCE_DIR}/../libOVIA")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../BUILD/${ARCHITECTURE}/${BUILDTYPE}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../BUILD/${ARCHITECTURE}/${BUILDTYPE}")
set(RUNTIME_OUTPUT_DIRECTORY       "${CMAKE_CURRENT_SOURCE_DIR}/../BUILD/${ARCHITECTURE}/${BUILDTYPE}")

set(OVIA_PublicHeaders
    "${OVIA_Dir}/include/OVIA.h"
)

set(OVIA_Sources
    "${OVIA_Dir}/src/OVIA.c"
	"${OVIA_Dir}/src/Common/AIFCommon.c"
	"${OVIA_Dir}/src/Common/BMPCommon.c"
	"${OVIA_Dir}/src/Common/PNMCommon.c"
	"${OVIA_Dir}/src/Common/W64Common.c"
	"${OVIA_Dir}/src/Common/WAVCommon.c"
	"${OVIA_Dir}/src/Common/PNGCommon.c"
	"${OVIA_Dir}/src/Common/FLACCommon.c"
    "${OVIA_Dir}/src/Decode/AIFDecoder.c"
    "${OVIA_Dir}/src/Decode/FLACDecoder.c"
    "${OVIA_Dir}/src/Decode/W64Decoder.c"
    "${OVIA_Dir}/src/Decode/WAVDecoder.c"
    "${OVIA_Dir}/src/Decode/BMPDecoder.c"
    "${OVIA_Dir}/src/Decode/PNGDecoder.c"
    "${OVIA_Dir}/src/Decode/PNMDecoder.c"
    "${OVIA_Dir}/src/Encode/AIFEncoder.c"
    "${OVIA_Dir}/src/Encode/FLACEncoder.c"
    "${OVIA_Dir}/src/Encode/W64Encoder.c"
    "${OVIA_Dir}/src/Encode/WAVEncoder.c"
    "${OVIA_Dir}/src/Encode/BMPEncoder.c"
    "${OVIA_Dir}/src/Encode/PNGEncoder.c"
    "${OVIA_Dir}/src/Encode/PNMEncoder.c"
)

if(NOT ${CMAKE_BUILD_TYPE})
  set(CMAKE_BUILD_TYPE "${MinSizeRel}")
endif()

add_subdirectory(../Dependencies/FoundationIO/Projects "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/../Dependencies/FoundationIO/BUILD")

add_library(OVIA ${OVIA_Sources} ${OVIA_PublicHeaders})

set_target_properties(OVIA PROPERTIES PREFIX "")

TARGET_LINK_LIBRARIES(OVIA FoundationIO)

if (${CMAKE_C_COMPILER_ID} MATCHES "AppleClang" OR ${CMAKE_C_COMPILER_ID} MATCHES "Clang")

set(CMAKE_C_FLAGS " \
${CMAKE_C_FLAGS} \
-fPIC \
-fPIE \
-fshort-enums \
-funroll-loops \
-funsigned-char \
-std=c17 \
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
-Wno-varargs \
-v \
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
-DDEBUG=1 \
-g \
-Ofast \
")

set(CMAKE_C_FLAGS_MINSIZEREL " \
${CMAKE_C_FLAGS} \
-Oz \
")

set(LINK_FLAGS ${LINK_FLAGS} "-flto=full")

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

elseif (${CMAKE_C_COMPILER_ID} MATCHES "GNU")

set(CMAKE_C_FLAGS " \
${CMAKE_C_FLAGS} \
-fPIC \
-fPIE \
-fshort-enums \
-funroll-loops \
-funsigned-char \
-std=c17 \
-Wall \
-Wextra \
-Wpedantic \
-Wno-ignored-attributes \
-Wno-padded \
-Wno-pointer-sign \
-Wno-sign-conversion \
-Wno-varargs \
-Wno-discarded-qualifier \
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
-DDEBUG=1 \
-g \
-Ofast \
")

set(CMAKE_C_FLAGS_MINSIZEREL " \
${CMAKE_C_FLAGS} \
-Os \
")

set(LINK_FLAGS ${LINK_FLAGS} "-flto=full")

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

elseif (${MAKE_C_COMPILER_ID} MATCHES "MSVC")

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

endif (${CMAKE_C_COMPILER_ID} MATCHES "AppleClang" OR ${CMAKE_C_COMPILER_ID} MATCHES "Clang")
