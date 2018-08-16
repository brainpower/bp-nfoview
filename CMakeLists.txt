project(bp-nfoview)
set(MAIN_VERSION "0.2.5")

cmake_minimum_required(VERSION 3.1)

option(USE_QT5 "Build using Qt5" ON)
option(WIN32 "Build for WIN32 environment..." OFF)

if(USE_QT5)
	find_package(Qt5Core REQUIRED)
	find_package(Qt5Widgets REQUIRED)

	set(BP_QT5 TRUE)
	message(STATUS "Found Qt5.")
else()
	find_package(Qt4 4.7)
	if(NOT ${QT_FOUND})
		message(STATUS "Recheck for Qt4 now with -qt4 postfix added...")

		set(QT_QMAKE_EXECUTABLE qmake-qt4 )
		set(QT_MOC_EXECUTABLE   moc-qt4 )
		set(QT_RCC_EXECUTABLE   rcc-qt4 )
		set(QT_UIC_EXECUTABLE   uic-qt4 )

		find_package(Qt4 4.7 REQUIRED)
	endif()
	message(STATUS "Found Qt4.")
endif()

if(NOT USE_QT5)
	include(${QT_USE_FILE})

	link_directories( ${QT_LIBRARY_DIR})
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)

# compiler optmization level
ADD_DEFINITIONS( -O3 )

# extra stuff needed with windows compilers
IF(WIN32)
	ADD_DEFINITIONS(-DWIN32 -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT)
	SET(GUI_TYPE WIN32)
	ADD_DEFINITIONS(-pipe -frtti -fexceptions -mthreads -Wall -enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc -Wl,-s -mthreads -Wl,-subsystem,windows -static)

	set(W32_LIBS mingw32 comdlg32 oleaut32 imm32 winmm winspool jpeg mng lcms tiff msimg32 png16 ssl crypto gdi32 crypt32 kernel32 user32 shell32 uuid ole32 advapi32 ws2_32 z lzma intl iconv)
ENDIF(WIN32)
IF(CMAKE_TOOLCHAIN_FILE)
ENDIF(CMAKE_TOOLCHAIN_FILE)


configure_file(cm_config.h.in config.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/src/bp-nfoview DESTINATION bin)
INSTALL(FILES bp-nfoview.desktop DESTINATION share/applications)
INSTALL(FILES img/bp-nfoview.png DESTINATION share/pixmaps)
