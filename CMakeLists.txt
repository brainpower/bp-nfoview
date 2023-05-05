# SPDX-FileCopyrightText: 2013-2023 brainpower <brainpower@mailbox.org>
# SPDX-License-Identifier: GPL-2.0-or-later OR MIT

cmake_minimum_required(VERSION 3.16)

project(bp-nfoview VERSION 0.3.1 LANGUAGES CXX)
set(MAIN_CPYRGT_YEAR "2010 - 2023")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_MAJOR_VERSION 5 CACHE STRING "Qt Version to build with")

option(BUILD_STATIC "Use static libraries" OFF)
option(DEBUG "Build in debug mode..." OFF)

include(GNUInstallDirs)


if (${QT_MAJOR_VERSION} STREQUAL 6)
  set(MAYBE_COMPAT Core5Compat)
endif()

find_package(Qt${QT_MAJOR_VERSION} 5.10 CONFIG REQUIRED COMPONENTS
  Core
  Gui
  Widgets
  ${MAYBE_COMPAT}
)

# forbid some old things
add_definitions(-DQT_NO_FOREACH)
#add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050d00)
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050f02)


if (DEBUG)
  # when debugging we want to see stdout on win32
  set(CMAKE_WIN32_EXECUTABLE OFF)
  # when developing we dont want warnings
  add_definitions(-Werror)
endif(DEBUG)


if (BUILD_STATIC)
  set(STATIC_BUILD ON FORCE)
  set(BUILD_SHARED_LIBS OFF FORCE)
  set(Qt${QT_MAJOR_VERSION}_USE_STATIC_LIBS ON)
  set(Qt${QT_MAJOR_VERSION}_USE_STATIC_RUNTIME ON)
  set(USE_STATIC_QT_BY_DEFAULT ON)
  # https://github.com/Martchus/PKGBUILDs/blob/master/cmake/mingw-w64-static/toolchain-mingw-static.cmake
  set(CMAKE_FIND_LIBRARY_SUFFIXES_OVERRIDE ".a;.lib")
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.lib")
  set(CMAKE_EXE_LINKER_FLAGS "$ENV{LDFLAGS} -static -static-libgcc -static-libstdc++" CACHE STRING "linker flags for static builds" FORCE)
  set(CMAKE_FIND_ROOT_PATH "/usr/x86_64-w64-mingw32/static;${CMAKE_FIND_ROOT_PATH}")
  set(Vulkan_LIBRARY "/usr/x86_64-w64-mingw32/lib/libvulkan.dll.a" CACHE FILEPATH "shared Vulkan IDC library")
  # workaround limitations in upstream pkg-config files and CMake find modules
  set(pkgcfg_lib_libbrotlicommon_brotlicommon "/usr/x86_64-w64-mingw32/lib/libbrotlicommon-static.a" CACHE INTERNAL "static libbrotlicommon")
  set(pkgcfg_lib_libbrotlienc_brotlienc "/usr/x86_64-w64-mingw32/lib/libbrotlienc-static.a;/usr/x86_64-w64-mingw32/lib/libbrotlicommon-static.a" CACHE INTERNAL "static libbrotliend")
  set(pkgcfg_lib_libbrotlidec_brotlidec "/usr/x86_64-w64-mingw32/lib/libbrotlidec-static.a;/usr/x86_64-w64-mingw32/lib/libbrotlicommon-static.a" CACHE INTERNAL "static libbrotlidec")
  set(libbrotlicommon_STATIC_LDFLAGS "${pkgcfg_lib_libbrotlicommon_brotlicommon}" CACHE INTERNAL "static libbrotlicommon")
  set(libbrotlienc_STATIC_LDFLAGS "${pkgcfg_lib_libbrotlienc_brotlienc}" CACHE INTERNAL "static libbrotliend")
  set(libbrotlidec_STATIC_LDFLAGS "${pkgcfg_lib_libbrotlidec_brotlidec}" CACHE INTERNAL "static libbrotlidec")
  set(FREETYPE_DEPENDENCIES "-lgraphite2;-lbz2;-lharfbuzz;-lfreetype;-lbrotlidec-static;-lbrotlicommon-static" CACHE INTERNAL "dependencies of static FreeType2 library")
  set(OPENSSL_USE_STATIC_LIBS ON)
  set(OPENSSL_DEPENDENCIES "-lws2_32;-lgdi32;-lcrypt32" CACHE INTERNAL "dependencies of static OpenSSL libraries")
  set(libwebp_DEPENDENCIES "-lsharpyuv" CACHE INTERNAL "deps of webp")
endif()

configure_file(src/config.hpp.in config.hpp @ONLY)


add_executable(${PROJECT_NAME})

if (QT_MAJOR_VERSION STREQUAL 6)
  target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
else()
  target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
endif()

target_sources(${PROJECT_NAME} PRIVATE
  src/AboutDialog.cpp
  src/BPNVMainWindow.cpp
  src/qcodepage437codec.cpp
  src/bp-nfoview.cpp
)
target_link_libraries(${PROJECT_NAME}
  Qt${QT_MAJOR_VERSION}::Core
  Qt${QT_MAJOR_VERSION}::Gui
  Qt${QT_MAJOR_VERSION}::Widgets
)
if (${QT_MAJOR_VERSION} STREQUAL 6)
  target_link_libraries(${PROJECT_NAME} Qt${QT_MAJOR_VERSION}::Core5Compat)
endif()

if (BUILD_STATIC)
  # fix webp thing not picking up it's dependencies correctly'
  target_link_libraries(${PROJECT_NAME} ${PC_WebP_STATIC_LIBRARIES})

  #target_sources(${PROJECT_NAME} PRIVATE src/qt_plugins_static.cpp)
  qt_import_plugins(${PROJECT_NAME}
    INCLUDE
      Qt${QT_MAJOR_VERSION}::QWindowsIntegrationPlugin
      Qt${QT_MAJOR_VERSION}::QSvgIconPlugin
    INCLUDE_BY_TYPE imageformats
      Qt${QT_MAJOR_VERSION}::QGifPlugin
      Qt${QT_MAJOR_VERSION}::QJpegPlugin
      Qt${QT_MAJOR_VERSION}::QWebpPlugin
    EXCLUDE_BY_TYPE
      sqldrivers networkinformation texttospeech sensors scxmldatamodel
      qmltooling qmllint position multimedia designer
  )
endif()


if (NOT WIN32)
  install(
    PROGRAMS data/bp-nfoview.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
  )
endif()

install(
  PROGRAMS data/bp-nfoview.png
  DESTINATION ${CMAKE_INSTALL_DATADIR}/pixmaps
)
install(
  TARGETS ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# if tests enabled, go build them
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  if(NOT WIN32)
    find_program(DESKTOP_FILE_VALIDATE NAMES desktop-file-validate)
    if(DESKTOP_FILE_VALIDATE)
      add_test(
        NAME "validate-desktop-file"
        COMMAND "${DESKTOP_FILE_VALIDATE}" "${PROJECT_SOURCE_DIR}/data/bp-nfoview.desktop"
      )
    endif()
  endif()
endif()
