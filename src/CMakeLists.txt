
set(HDRS bp-nfoview.h qcodepage437codec.h BPNVMainWindow.hpp AboutDialog.hpp)
set(SRCS bp-nfoview.cpp qcodepage437codec.cpp bp-nfoview.qrc BPNVMainWindow.cpp AboutDialog.cpp)

if(NOT USE_QT5)
	include(${QT_USE_FILE})
endif()

ADD_EXECUTABLE(bp-nfoview ${HDRS} ${SRCS})

target_compile_features(bp-nfoview PUBLIC cxx_auto_type cxx_nullptr ) # cxx_memory_shared_ptr)

IF(WIN32)
	SET_TARGET_PROPERTIES(bp-nfoview PROPERTIES LINK_FLAGS "-Wl,-enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc -Wl,-s -mthreads -Wl,-subsystem,windows -static")
	SET_TARGET_PROPERTIES(bp-nfoview PROPERTIES WIN32_EXECUTABLE ON)
	if(USE_QT5)
		target_link_libraries(bp-nfoview ${W32_LIBS})
		qt5_use_modules(bp-nfoview Core Widgets)
	else()
		target_link_libraries(bp-nfoview ${QT_LIBRARIES} ${W32_LIBS})
	endif()
ELSE()
	if(USE_QT5)
		qt5_use_modules(bp-nfoview Core Widgets)
	else()
		target_link_libraries(bp-nfoview  ${QT_LIBRARIES})
	endif()
ENDIF(WIN32)
