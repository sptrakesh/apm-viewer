#include "mainwindow.hpp"
#include "logger/logger.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
  qInstallMessageHandler( logger::formatMessage );
  setbuf( stdout, nullptr );

  QApplication a( argc, argv );
  QApplication::setOrganizationName( "Rakesh Vidyadharan" );
  QApplication::setOrganizationDomain( "com.sptci" );
  QApplication::setApplicationName( "APM Viewer" );

  MainWindow w;
  w.show();
  return a.exec();
}
