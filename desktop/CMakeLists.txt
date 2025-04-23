cmake_minimum_required(VERSION 3.19)
project(desktop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(desktop
  WIN32 MACOSX_BUNDLE
  date/date.hpp
  date/date.cpp
  model/csv.hpp
  model/node.hpp
  model/node.cpp
  model/treemodel.hpp
  model/treemodel.cpp
  model/parser.cpp
  model/parser.hpp
  logger/logger.hpp
  logger/logger.cpp
  mainwindow.hpp
  mainwindow.ui
  mainwindow.cpp
  main.cpp
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
