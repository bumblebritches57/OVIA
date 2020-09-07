cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Ok, so new structure: Globals, followed by Options, followed by config-specific variables.

project(OVIA C)
include(CTest)

if(DEFINED CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug")
elseif(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif(DEFINED CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE STREQUAL "")



include(../Dependencies/FoundationIO/Projects/DetectArchitecture.cmake)

FindTargetArchitecture(TargetArchitecture)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../BUILD/${TargetArchitecture}/${CMAKE_BUILD_TYPE}")
set(CMAKE_OUTPUT_DIRECTORY         "${CMAKE_CURRENT_SOURCE_DIR}/../BUILD/${TargetArchitecture}/${CMAKE_BUILD_TYPE}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../BUILD/${TargetArchitecture}/${CMAKE_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../BUILD/${TargetArchitecture}/${CMAKE_BUILD_TYPE}")
set(OVIA_Library_Dir               "${CMAKE_CURRENT_SOURCE_DIR}/../Library")
set(OVIA_Test_Dir                  "${CMAKE_CURRENT_SOURCE_DIR}/../Tests")
set(OVIA_Resource_Dir              "${CMAKE_CURRENT_SOURCE_DIR}/../Resources")

set(OVIA_SOURCE
    "${OVIA_Library_Dir}/src/OVIA.c"
    "${OVIA_Library_Dir}/src/MediaIO.c"
    "${OVIA_Library_Dir}/src/CodecIO/FLACCodec.c"
    "${OVIA_Library_Dir}/src/CodecIO/JPEGCodec.c"
    "${OVIA_Library_Dir}/src/CodecIO/PNGCodec.c"
    "${OVIA_Library_Dir}/src/CodecIO/PCMCodec.c"
    "${OVIA_Library_Dir}/src/EntropyIO/EntropyIO.c"
    "${OVIA_Library_Dir}/src/EntropyIO/ArthimeticEntropy.c"
    "${OVIA_Library_Dir}/src/EntropyIO/FlateEntropy.c"
    "${OVIA_Library_Dir}/src/EntropyIO/HuffmanEntropy.c"
    "${OVIA_Library_Dir}/src/StreamIO/AIFStream.c"
    "${OVIA_Library_Dir}/src/StreamIO/BMPStream.c"
    "${OVIA_Library_Dir}/src/StreamIO/PNMStream.c"
    "${OVIA_Library_Dir}/src/StreamIO/RIFFStream.c"
    "${OVIA_Library_Dir}/src/StreamIO/W64Stream.c"
    "${OVIA_Library_Dir}/src/TagIO/TagIO.c"
    "${OVIA_Library_Dir}/src/TagIO/AIFTags.c"
    "${OVIA_Library_Dir}/src/TagIO/APETags.c"
    "${OVIA_Library_Dir}/src/TagIO/ID3Tags.c"
    "${OVIA_Library_Dir}/src/TagIO/VorbisTags.c"
    "${OVIA_Library_Dir}/src/TagIO/WAVTags.c"
    "${OVIA_Library_Dir}/src/TransformIO/RCT.c"
    "${OVIA_Library_Dir}/src/TransformIO/YCoCgR.c"
)

if(CMAKE_HOST_UNIX)
    add_definitions(-D_LARGEFILE_SOURCE)
    add_definitions(-D_LARGEFILE64_SOURCE)
    add_definitions(-D_FILE_OFFSET_BITS=64)
    add_definitions(-D__STDC_WANT_LIB_EXT1__)
elseif(CMAKE_HOST_WIN32)
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
    add_definitions(-D_ATL_ALLOW_CHAR_UNSIGNED)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(CMAKE_HOST_UNIX)

find_package(Git)

if(GIT_FOUND)
    file(READ "../Library/include/Private/Version.h" OVIA_Version_Header)

    string(REGEX MATCH "#define OVIA_Version_Major [0-9]+" _ "${OVIA_Version_Header}")
    string(REGEX MATCH "[0-9]+" VERSION_MAJOR "${_}")

    string(REGEX MATCH "#define OVIA_Version_Minor [0-9]+" _ "${OVIA_Version_Header}")
    string(REGEX MATCH "[0-9]+" VERSION_MINOR "${_}")

    string(REGEX MATCH "#define OVIA_Version_Patch [0-9]+" _ "${OVIA_Version_Header}")
    string(REGEX MATCH "[0-9]+" VERSION_PATCH "${_}")


    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --verify master WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE GIT_COMMIT_ID RESULTS_VARIABLE STDOUT ERROR_VARIABLE STDERR)
    string(REGEX REPLACE "\n$" "" GIT_COMMIT_ID_STRIPPED "${GIT_COMMIT_ID}")
    string(TOUPPER "${GIT_COMMIT_ID_STRIPPED}" GIT_COMMIT_ID_STRIPPED_CAPS)

    set(OVIA_VERSION_STRING "")
    string(APPEND OVIA_VERSION_STRING "${VERSION_MAJOR}" "." "${VERSION_MINOR}" "." "${VERSION_PATCH}" ":" "${GIT_COMMIT_ID_STRIPPED_CAPS}")

    set(FoundationIO_VERSION_RESOURCE "")
    string(APPEND OVIA_VERSION_RESOURCE "${VERSION_MAJOR}" "," "${VERSION_MINOR}" "," "${VERSION_PATCH}" "," "${GIT_COMMIT_ID_STRIPPED_CAPS}")
endif(GIT_FOUND)

configure_file("${OVIA_Resource_Dir}/OVIA.pc.in" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Resources/OVIA.pc")
configure_file("${OVIA_Resource_Dir}/OVIA.rc.in" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Resources/OVIA.rc")

option(BUILD_AS_SHARED_LIBRARY "Should OVIA be compiled as a dynamic library (.dylib/.dll/.so)?" ON)

if(BUILD_AS_SHARED_LIBRARY MATCHES ON)
    add_library(OVIA SHARED ${OVIA_SOURCE})
elseif(BUILD_AS_SHARED_LIBRARY MATCHES OFF)
    add_library(OVIA STATIC ${OVIA_SOURCE})
endif(BUILD_AS_SHARED_LIBRARY MATCHES ON)

set_target_properties(OVIA PROPERTIES PREFIX "")

target_include_directories(OVIA PUBLIC "${OVIA_Library_Dir}/include")

if(NOT DEFINED CMAKE_INSTALL_PREFIX AND CMAKE_HOST_UNIX)
    set(CMAKE_INSTALL_PREFIX "/usr/local")
elseif(NOT DEFINED CMAKE_INSTALL_PREFIX AND CMAKE_HOST_WIN32)
    set(CMAKE_INSTALL_PREFIX "%ProgramFiles%\OVIA")
endif(NOT DEFINED CMAKE_INSTALL_PREFIX AND CMAKE_HOST_UNIX)

install(TARGETS OVIA                          DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES  ${OVIA_PUBLIC_HEADERS}         DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Resources/OVIA.pc" DESTINATION "${CMAKE_INSTALL_PREFIX}/share/pkgconfig")

if(BUILD_TESTING)
    enable_testing()

    add_executable(Test_CodecIO                     "${OVIA_Test_Dir}/Test_CodecIO.c")
    add_executable(Test_EntropyIO                   "${OVIA_Test_Dir}/Test_EntropyIO.c")
    add_executable(Test_StreamIO                    "${OVIA_Test_Dir}/Test_StreamIO.c")
    add_executable(Test_TagIO                       "${OVIA_Test_Dir}/Test_TagIO.c")
    add_executable(Test_TransformIO                 "${OVIA_Test_Dir}/Test_TransformIO.c")
    add_executable(Test_OVIA                        "${OVIA_Test_Dir}/Test_OVIA.c")

    target_link_libraries(Test_CodecIO              OVIA)
    target_link_libraries(Test_EntropyIO            OVIA)
    target_link_libraries(Test_StreamIO             OVIA)
    target_link_libraries(Test_TagIO                OVIA)
    target_link_libraries(Test_TransformIO          OVIA)
    target_link_libraries(Test_OVIA                 OVIA)

    add_test(Test_CodecIO                           "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Test_CodecIO")
    add_test(Test_EntropyIO                         "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Test_EntropyIO")
    add_test(Test_StreamIO                          "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Test_StreamIO")
    add_test(Test_TagIO                             "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Test_TagIO")
    add_test(Test_TransformIO                       "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Test_TransformIO")
    add_test(Test_OVIA                              "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Test_OVIA")

    install(TARGETS Test_CodecIO                    DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
    install(TARGETS Test_EntropyIO                  DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
    install(TARGETS Test_StreamIO                   DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
    install(TARGETS Test_TagIO                      DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
    install(TARGETS Test_TransformIO                DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
    install(TARGETS Test_OVIA                       DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")

    add_custom_command(TARGET OVIA POST_BUILD COMMAND ctest ARGS --output-on-failure WORKING_DIRECTORY ${CMAKE_OUTPUT_DIRECTORY})

endif(BUILD_TESTING)

install(TARGETS OVIA DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES ${OVIA_PUBLICHEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

add_subdirectory(../Dependencies/FoundationIO/Projects "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/../Dependencies/FoundationIO/BUILD")

set_target_properties(OVIA PROPERTIES PREFIX "")

target_link_libraries(OVIA PUBLIC FoundationIO)

if(${CMAKE_C_COMPILER_ID} STREQUAL "AppleClang" OR ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")

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

if(CMAKE_HOST_APPLE)
set(LINK_FLAGS ${LINK_FLAGS} " \
-dead_strip_dylibs
")
endif(CMAKE_HOST_APPLE)

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

elseif(${CMAKE_C_COMPILER_ID} STREQUAL "GNU")

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

if(CMAKE_HOST_APPLE)
set(LINK_FLAGS ${LINK_FLAGS} " \
-dead_strip_dylibs
")
endif(CMAKE_HOST_APPLE)

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

elseif(${CMAKE_C_COMPILER_ID} STREQUAL "MSVC")

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

endif(${CMAKE_C_COMPILER_ID} STREQUAL "AppleClang" OR ${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
