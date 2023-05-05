# SPDX-FileCopyrightText: 2013-2023 brainpower <brainpower@mailbox.org>
# SPDX-License-Identifier: GPL-2.0-or-later OR MIT

cmake_minimum_required(VERSION 3.16)

project(bp-nfoview VERSION 0.3.1 LANGUAGES CXX)
set(MAIN_CPYRGT_YEAR "2010 - 2023")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(QT_MAJOR_VERSION 5 CACHE STRING "Qt Version to build with")

option(BUILD_STATIC "Use static libraries" OFF)
option(DEBUG "Build in debug mode..." OFF)

include(GNUInstallDirs)


if (${QT_MAJOR_VERSION} STREQUAL 6)
  set(MAYBE_COMPAT Core5Compat)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()


if (BUILD_STATIC)
  set(STATIC_BUILD ON) # for config.hpp

  #set(CMAKE_FIND_LIBRARY_SUFFIXES_OVERRIDE ".a;.lib")
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.lib")

  set(USE_STATIC_QT_BY_DEFAULT ON)
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

  if (${QT_MAJOR_VERSION} STREQUAL 5)
    add_definitions( -DQT_STATICPLUGIN )
    target_link_libraries(${PROJECT_NAME}
      Qt${QT_MAJOR_VERSION}::QWindowsIntegrationPlugin
      Qt${QT_MAJOR_VERSION}::QWindowsVistaStylePlugin
      Qt${QT_MAJOR_VERSION}::QSvgIconPlugin
      Qt${QT_MAJOR_VERSION}::QGifPlugin
      Qt${QT_MAJOR_VERSION}::QJpegPlugin
      Qt${QT_MAJOR_VERSION}::QWebpPlugin
    )
    target_sources(${PROJECT_NAME} PRIVATE src/qt_plugins_static.cpp)
  endif()

  qt_import_plugins(${PROJECT_NAME}
    INCLUDE
      Qt${QT_MAJOR_VERSION}::QWindowsIntegrationPlugin
      Qt${QT_MAJOR_VERSION}::QWindowsVistaStylePlugin
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
