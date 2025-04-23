cmake_minimum_required(VERSION 3.19)
project(desktop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

if (APPLE)
  set(MACOSX_BUNDLE_ICON_FILE apm.icns)
  set(app_icon "${CMAKE_CURRENT_SOURCE_DIR}/resources/apm.icns")
  set_source_files_properties(${app_icon} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif (APPLE)

if (WIN32)
  set(app_icon "${CMAKE_CURRENT_SOURCE_DIR}/resources/winicon.rc")
endif (WIN32)

qt_add_executable(desktop
  WIN32 MACOSX_BUNDLE
  src/date/date.hpp
  src/date/date.cpp
  src/model/csv.hpp
  src/model/node.hpp
  src/model/node.cpp
  src/model/treemodel.hpp
  src/model/treemodel.cpp
  src/model/parser.cpp
  src/model/parser.hpp
  src/logger/logger.hpp
  src/logger/logger.cpp
  src/mainwindow.hpp
  src/mainwindow.ui
  src/mainwindow.cpp
  src/main.cpp
  ${app_icon}
)

target_link_libraries(desktop
  PRIVATE
    Qt::Core
    Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS desktop
  BUNDLE  DESTINATION .
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
  TARGET desktop
  OUTPUT_SCRIPT deploy_script
  NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
