# SPDX-FileCopyrightText: 2013-2024 brainpower <brainpower@mailbox.org>
# SPDX-License-Identifier: GPL-2.0-or-later OR MIT

cmake_minimum_required(VERSION 3.16..3.30)

project(bp-nfoview
  VERSION 0.3.1
  LANGUAGES CXX
  DESCRIPTION "A Simple NFO-Viewer"
)
set(MAIN_CPYRGT_YEAR "2010 - 2024")

###
# Options
##

set(QT_MAJOR_VERSION 6 CACHE STRING "Qt Version to build with")

option(BUILD_STATIC "Use static libraries and produce a static binary" OFF)
option(WIN32_CONSOLE "Whether to always show a console on windows for debugging" OFF)

###
# Settings
##

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (${QT_MAJOR_VERSION} STREQUAL 6)
  set(QT_MIN_VERSION 6.2)
  # disable stuff that was deprecated since the specified version
  set(QT_DISABLE_DEPRECATED "0x060600") # 6.6
  set(MAYBE_COMPAT Core5Compat)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
  set(QT_MIN_VERSION 5.10)
  set(QT_DISABLE_DEPRECATED "0x050f00") # 5.15
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if (BUILD_STATIC)
  set(STATIC_BUILD ON) # for config.hpp

  #set(CMAKE_FIND_LIBRARY_SUFFIXES_OVERRIDE ".a;.lib")
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.lib")

  set(USE_STATIC_QT_BY_DEFAULT ON)
endif()

include(GNUInstallDirs)


###
# Lookups
##

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION}
  CONFIG
  REQUIRED
  COMPONENTS
    Core
    Gui
    Widgets
    ${MAYBE_COMPAT}
)

#if (DEBUG) # TODO: is there a target based option for this??
#  # when debugging we want to see stdout on win32
#  set(CMAKE_WIN32_EXECUTABLE OFF)
#elseif(WIN32)
#  set(CMAKE_WIN32_EXECUTABLE ON)
#endif()

configure_file(src/config.hpp.in config.hpp @ONLY)


###
# Targets
##

add_executable(bp-nfoview)

if (QT_MAJOR_VERSION STREQUAL 6)
  target_compile_features(bp-nfoview PUBLIC cxx_std_17)
else()
  target_compile_features(bp-nfoview PUBLIC cxx_std_14)
endif()

target_compile_definitions(bp-nfoview
  PRIVATE
    # forbid some old things
    QT_NO_FOREACH
    QT_DISABLE_DEPRECATED_BEFORE=${QT_DISABLE_DEPRECATED}
    # and warn about soon to be old things
    QT_DEPRECATED_WARNINGS
)

# when developing we want warnings to be errors
target_compile_options(bp-nfoview PRIVATE "$<$<CONFIG:Debug>:-Werror>")

if (WIN32_CONSOLE)
  # force WinMain to off, so a console is always shown
  set_target_properties(bp-nfoview PROPERTIES WIN32_EXECUTABLE OFF)
else()
  set_target_properties(bp-nfoview
    PROPERTIES
      # when debugging we want to see stdout on win32, so switch WinMain off
      WIN32_EXECUTABLE "$<IF:$<CONFIG:Debug>,OFF,ON>"
  )
endif()

target_sources(bp-nfoview
  PRIVATE
    src/AboutDialog.cpp
    src/BPNVMainWindow.cpp
    src/qcodepage437codec.cpp
    src/bp-nfoview.cpp
)
target_link_libraries(bp-nfoview
  PUBLIC
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Gui
    Qt${QT_MAJOR_VERSION}::Widgets
)
if (${QT_MAJOR_VERSION} STREQUAL 6)
  target_link_libraries(bp-nfoview PUBLIC Qt${QT_MAJOR_VERSION}::Core5Compat)
endif()

if (BUILD_STATIC)
  # fix webp thing not picking up it's dependencies correctly'
  target_link_libraries(bp-nfoview PUBLIC ${PC_WebP_STATIC_LIBRARIES})

  if (${QT_MAJOR_VERSION} STREQUAL 5)
    target_compile_definitions(bp-nfoview PRIVATE QT_STATICPLUGIN)
    target_link_libraries(bp-nfoview
      PRIVATE
        Qt${QT_MAJOR_VERSION}::QWindowsIntegrationPlugin
        Qt${QT_MAJOR_VERSION}::QWindowsVistaStylePlugin
        Qt${QT_MAJOR_VERSION}::QSvgIconPlugin
        Qt${QT_MAJOR_VERSION}::QGifPlugin
        Qt${QT_MAJOR_VERSION}::QJpegPlugin
        Qt${QT_MAJOR_VERSION}::QWebpPlugin
    )
    target_sources(bp-nfoview PRIVATE src/qt_plugins_static.cpp)
  endif()

  qt_import_plugins(bp-nfoview
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
  FILES data/bp-nfoview.png
  DESTINATION ${CMAKE_INSTALL_DATADIR}/pixmaps
)
install(
  TARGETS bp-nfoview
)

# if tests enabled, go build them
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  if(BUILD_TESTING)
    # find_package(Catch2)
    #
    # if (NOT Catch2_FOUND)
    #   if(EXISTS subprojects/Catch2/CMakeLists.txt)
    #     add_subdirectory(subprojects/Catch2)
    #     set(Catch2_FOUND ON)
    #   endif()
    # endif()
    #
    # if (Catch2_FOUND)
    #   include(Catch)
    #   add_executable(tests tests/test.cpp)
    #   target_include_directories(tests PRIVATE "${PROJECT_SOURCE_DIR}/src")
    #   target_link_libraries(tests PRIVATE
    #     Catch2::Catch2
    #     Qt${QT_MAJOR_VERSION}::Core
    #   )
    #   if (${QT_MAJOR_VERSION} STREQUAL 6)
    #     target_link_libraries(tests PRIVATE Qt${QT_MAJOR_VERSION}::Core5Compat)
    #   endif()
    #   catch_discover_tests(tests)
    # else()
    #   error("Need Catch2 for tests. Install catch2 system package"
    #         " or clone the git submodule at subprojects/Catch2 (git submodule update --init)")
    # endif()

    if(NOT WIN32)
      find_program(DESKTOP_FILE_VALIDATE NAMES desktop-file-validate)
      if(DESKTOP_FILE_VALIDATE)
        add_test(
          NAME "validate-desktop-file"
          COMMAND "${DESKTOP_FILE_VALIDATE}" "${PROJECT_SOURCE_DIR}/data/bp-nfoview.desktop"
        )
      endif(DESKTOP_FILE_VALIDATE)
    endif(NOT WIN32)
  endif(BUILD_TESTING)

  ###
  #  Packaging
  ##
  set(CPACK_PACKAGE_CONTACT "brainpower <brainpower@mailbox.org>")
  set(PACKAGE_LONG_DESCRIPTION "A Simple NFO-Viewer
bp-nfoview is a simple viewer for NFO files, of which most are ASCII art in
the CP437 codepage. The advantages of using NFO Viewer instead of a
text editor are preset font, color and encoding settings.")

  # rpm
  set(CPACK_RPM_PACKAGE_DESCRIPTION "${PACKAGE_LONG_DESCRIPTION}")
  set(CPACK_RPM_FILE_NAME "RPM-DEFAULT")
  #set(CPACK_RPM_PACKAGE_LICENSE "GPL-2.0-or-later")

  # debian
  set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "${PACKAGE_LONG_DESCRIPTION}")
  set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
  set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
  set(CPACK_DEBIAN_PACKAGE_PRIORITY "extra")
  set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
  # fallback
  if(NO_SHLIBDEPS)
    if (QT_MAJOR_VERSION STREQUAL 6)
      set(CPACK_DEBIAN_PACKAGE_DEPENDS
        "libc6, libgcc-s1, libqt6core5compat6, libqt6core6, libqt6gui6, libqt6widgets6, libstdc++6"
      )
    else()
      set(CPACK_DEBIAN_PACKAGE_DEPENDS
        "libc6, libgcc-s1, libqt5core5a, libqt5gui5, libqt5widgets5, libstdc++6"
      )
    endif()
  endif()

  set(CPACK_BINARY_STGZ OFF)
  set(CPACK_BINARY_TGZ OFF)
  set(CPACK_BINARY_TZ OFF)
  if(WIN32)
    if(PACKAGE_ALL)
      set(CPACK_BINARY_NSIS ON)
      set(CPACK_BINARY_ZIP ON)
    endif()
  else()
    if(PACKAGE_ALL)
      set(CPACK_BINARY_STGZ ON)
      set(CPACK_BINARY_TGZ ON)
      set(CPACK_BINARY_DEB ON)
      set(CPACK_BINARY_RPM ON)
    endif()
  endif()
  # needs to be after defining CPACK_* variables
  include(CPack)
endif()
