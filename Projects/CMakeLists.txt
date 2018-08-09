cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(libOVIA)

if (UNIX)
set(libMATH -lm)
add_definitions(-D_LARGEFILE_SOURCE)
add_definitions(-D_LARGEFILE64_SOURCE)
add_definitions(-D_FILE_OFFSET_BITS=64)
else (WIN32)
set(libMATH)
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)
endif

set(OVIA_Dir "${CMAKE_CURRENT_SOURCE_DIR}")

set(ARCHIVE_OUTPUT_DIRECTORY "${OVIA_Dir}/../BUILD/${ARCHITECTURE}/${BUILDTYPE})

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

if (CMAKE_C_COMPILER_ID == AppleClang || CMAKE_C_COMPILER_ID == Clang || CMAKE_C_COMPILER_ID == GNU)
	set(LINK_FLAGS "${LINK_FLAGS}" -flto)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" -std=c11 -ferror-limit=1024 -Weverything -fcolor-diagnostics -fconst-strings -fshort-enums -vectorize-loops -funroll-loops -funsigned-char -Wno-c99-compat -fstrict-enums)

    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS}" "${CMAKE_C_FLAGS_DEBUG} -g -O1")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS}" "${CMAKE_C_FLAGS_RELEASE} -Os")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS}" "${CMAKE_C_FLAGS_RELWITHDEBINFO} -g -Ofast")
	set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS}" "${CMAKE_C_FLAGS_MINSIZEREL} -Os")
endif(CMAKE_C_COMPILER_ID == AppleClang || CMAKE_C_COMPILER_ID == Clang || CMAKE_C_COMPILER_ID == GNU)

if (CMAKE_C_COMPILER_ID == MSVC)
    set(LINK_FLAGS "${LINK_FLAGS}" -flto)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" -std=c11 -ferror-limit=1024 -Weverything -fcolor-diagnostics -fconst-strings -fshort-enums -vectorize-loops -funroll-loops -funsigned-char -Wno-c99-compat -fstrict-enums)



/MP /FR"E:\FoundationIO\Projects\..\BUILD\x64\Release\Intermediate\" /GS- /Zc:rvalueCast /TC /Qpar /GL /analyze- /W3 /Gy /FAu /Zc:wchar_t /Qspectre /Zi /Gm- /Ox /Ob2 /sdl- /Fd"E:\FoundationIO\Projects\..\BUILD\x64\Release\Intermediate\FoundationIO.pdb" /Zc:inline /fp:precise /D "_LIB" /D "_UNICODE" /D "UNICODE" /fp:except- /errorReport:send /GF /GT /WX- /Zc:forScope /GR /arch:SSE2 /Gd /Oy- /MT /openmp /std:c++17 /FC /Fa"E:\FoundationIO\Projects\..\BUILD\x64\Release\Intermediate\" /nologo /Fo"E:\FoundationIO\Projects\..\BUILD\x64\Release\Intermediate\" /Ot /diagnostics:caret 

	set(LINK_FLAGS "${LINK_FLAGS}" -flto)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" -std=c11 -ferror-limit=1024 -Weverything -fcolor-diagnostics -fconst-strings -fshort-enums -vectorize-loops -funroll-loops -funsigned-char -Wno-c99-compat -fstrict-enums)

	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS}" "${CMAKE_C_FLAGS_DEBUG} -g -O1")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS}" "${CMAKE_C_FLAGS_RELEASE} -Os")
	set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS}" "${CMAKE_C_FLAGS_RELWITHDEBINFO} -g -Ofast")
	set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS}" "${CMAKE_C_FLAGS_MINSIZEREL} -Os")
endif(CMAKE_C_COMPILER_ID == MSVC)
