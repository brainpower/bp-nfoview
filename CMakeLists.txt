project(bp-nfoview)

cmake_minimum_required(VERSION 2.8)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

option(WIN32 "Build for WIN32 environment..." OFF)
ADD_DEFINITIONS(-O2)

IF(WIN32)
	ADD_DEFINITIONS(-DWIN32 -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT)
	SET(GUI_TYPE WIN32)
	ADD_DEFINITIONS(-pipe -frtti -fexceptions -mthreads -Wall -enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc -Wl,-s -mthreads -Wl,-subsystem,windows -static)

	set(W32_LIBS mingw32 comdlg32 oleaut32 imm32 winmm winspool jpeg mng lcms tiff msimg32 png16 ssl crypto gdi32 crypt32 kernel32 user32 shell32 uuid ole32 advapi32 ws2_32 z lzma intl iconv)
ENDIF(WIN32)
IF(CMAKE_TOOLCHAIN_FILE)
ENDIF(CMAKE_TOOLCHAIN_FILE)

add_subdirectory(src)

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/src/bp-nfoview DESTINATION bin)
INSTALL(FILES bp-nfoview.desktop DESTINATION share/applications)
INSTALL(FILES img/bp-nfoview.png DESTINATION share/pixmaps)
