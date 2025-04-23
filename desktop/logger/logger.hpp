#pragma once

#include <QtCore/QtLogging>

namespace logger
{
  QString logFile();
  void formatMessage( QtMsgType type, const QMessageLogContext& context, const QString& msg );
}
