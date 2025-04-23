#include "view/mainwindow.hpp"
#include "logger/logger.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
  qInstallMessageHandler( logger::formatMessage );
  setbuf( stdout, nullptr );

  QApplication::setOrganizationName( "Rakesh Vidyadharan" );
  QApplication::setOrganizationDomain( "com.sptci" );
  QApplication::setApplicationName( "APM Viewer" );
  QApplication a( argc, argv );

  view::MainWindow w;
  w.show();
  return a.exec();
}
