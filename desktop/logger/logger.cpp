#include "logger.hpp"

#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include <QtCore/QStandardPaths>
#include <QtCore/QTextStream>

#include <mutex>
#include <print>

namespace
{
  namespace plogger
  {
    struct Logger
    {
      Logger() : stream( &outFile )
      {
        outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        std::println( "Logging to file: {}", outFile.fileName().toLocal8Bit().constData() );
      }

      void log(const QString& message)
      {
        std::lock_guard lock( mutex );
        stream << message << Qt::endl;
      }

      ~Logger()
      {
        outFile.close();
        std::println( "Closing log file: {}", outFile.fileName().toLocal8Bit().constData() );
      }

      QFile outFile{ logger::logFile() };
      QTextStream stream;
      std::mutex mutex;
    };
  }
}

QString logger::logFile()
{
  return QString("%1/apm-desktop.log").arg( QStandardPaths::writableLocation( QStandardPaths::TempLocation ) );
}

void logger::formatMessage( QtMsgType type, const QMessageLogContext& context, const QString& msg )
{
  static plogger::Logger logger;

  const auto localMsg = msg.toLocal8Bit();
  const auto ts = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);
  const auto time = ts.toLocal8Bit();
  switch (type) {
  case QtDebugMsg:
    {
      std::println( "{} - {} [Debug]: {} ({}:{}, {})", time.constData(), context.category,
        localMsg.constData(), context.file, context.line, context.function );
      const auto message = QString( "%1 - %2 Debug: %3" ).arg( ts ).arg( context.category ).arg( msg );
      logger.log( message );
    }
      break;
  case QtInfoMsg:
    {
      std::println( "{} - {} [Info]: {} ({}:{}, {})", time.constData(), context.category,
        localMsg.constData(), context.file, context.line, context.function );
      const auto message = QString("%1 - %2 Info: %3").arg(ts).arg(context.category).arg(msg);
      logger.log(message);
    }
    break;
  case QtWarningMsg:
    {
      std::println( "{} - {} [Warning]: {} ({}:{}, {})", time.constData(), context.category,
        localMsg.constData(), context.file, context.line, context.function );
      const auto message = QString("%1 - %2 Warning: %3").arg(ts).arg(context.category).arg(msg);
      logger.log(message);
    }
    break;
  case QtCriticalMsg:
    {
      std::println( "{} - {} [Critical]: {} ({}:{}, {})", time.constData(), context.category,
        localMsg.constData(), context.file, context.line, context.function );
      const auto message = QString("%1 - %2 Critical: %3").arg(ts).arg(context.category).arg(msg);
      logger.log(message);
    }
    break;
  case QtFatalMsg:
    {
      std::println( "{} - {} [Fatal]: {} ({}:{}, {})", time.constData(), context.category,
        localMsg.constData(), context.file, context.line, context.function );
      const auto message = QString("%1 - %2 Fatal: %3").arg(ts).arg(context.category).arg(msg);
      logger.log(message);
      exit(1);
    }
  }
}
