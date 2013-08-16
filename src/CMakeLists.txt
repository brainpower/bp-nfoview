include(${QT_USE_FILE})

set(HDRS bp-nfoview.h dialog_about.h dialog_charbox.h qcodepage437codec.h)
set(SRCS bp-nfoview.cpp dialog_about.cpp dialog_charbox.cpp qcodepage437codec.cpp)

qt4_wrap_cpp(bp_nfoview_MOC_SRCS bp-nfoview.h dialog_about.h)
QT4_ADD_RESOURCES(bp_nfoview_RCC_SRCS bp-nfoview.qrc)

ADD_EXECUTABLE(bp-nfoview ${HDRS} ${SRCS} ${bp_nfoview_MOC_SRCS} ${bp_nfoview_RCC_SRCS})

IF(WIN32)
SET_TARGET_PROPERTIES(bp-nfoview PROPERTIES LINK_FLAGS "-Wl,-enable-stdcall-fixup -Wl,-enable-auto-import -Wl,-enable-runtime-pseudo-reloc -Wl,-s -mthreads -Wl,-subsystem,windows -static")
target_link_libraries(bp-nfoview ${QT_LIBRARIES} ${W32_LIBS})
ELSE()
target_link_libraries(bp-nfoview  ${QT_LIBRARIES})
ENDIF(WIN32)
