project (phone)
cmake_minimum_required (VERSION 2.6)

if (MSVC)
  add_definitions (/MT)
  add_definitions (/MP)
  add_definitions (/D_CRT_SECURE_NO_WARNINGS)
  set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:LIBCMT")
  set (phone_libs
    imm32.lib
#   msimg32.lib
    winmm.lib
    ws2_32.lib
  )
endif (MSVC)

find_package (Qt4 REQUIRED QtMain QtCore QtGui QtXml QtNetwork)
enable_testing ()

include_directories (${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set (phone_SRCS
  src/phone.h src/phone.cpp
  src/settings.h src/settings.cpp

  filetree/filetree.h filetree/filetree.cpp
  filetree/main.cpp filetree/mainwindow.h filetree/mainwindow.cpp

  proc/load_input_file.h
  proc/load_input_file.cpp

  proc/save_output_file.h
  proc/save_output_file.cpp

  proc/phone_table_model.h
  proc/phone_table_model.cpp

  enc/encoding_utils.h
  enc/encoding_utils.cpp
)

if (UNIX)
  add_definitions (-DSIMULATOR_USE_ICONV)
endif (UNIX)

add_definitions (${QT_DEFINITIONS})
include_directories (${CMAKE_BINARY_DIR})
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)

#qt4_automoc (${phone_SRCS})

qt4_wrap_cpp (phone_SRCS_MOC filetree/mainwindow.h
  proc/phone_table_model.h proc/load_input_file.h src/phone.h)
qt4_wrap_ui (phone_SRCS_UI filetree/mainwindow.ui)
qt4_add_resources (phone_SRCS_RES queries.qrc)

add_executable (phone WIN32 ${phone_SRCS} ${phone_SRCS_MOC} ${phone_SRCS_UI} ${phone_SRCS_RES})
target_link_libraries (phone ${QT_QTMAIN_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY} ${QT_QTXMLPATTERNS_LIBRARY} ${QT_QTNETWORK_LIBRARY}
  ${phone_libs})

if (UNIX)
  add_custom_target (gitk    COMMAND gitk --all WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif (UNIX)
