#include "mainwindow.hpp"
#include "ui_MainWindow.h"
#include "model/parser.hpp"
#include "model/treemodel.hpp"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QLoggingCategory>
#include <QtCore/QMimeData>
#include <QtCore/QThread>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QStandardItemModel>
#include <QtWidgets/QFileDialog>

#include "date/date.hpp"

Q_LOGGING_CATEGORY( MAIN_WINDOW, "MainWindow" )

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ) , ui( new Ui::MainWindow )
{
  ui->setupUi( this );
  ui->tableView->horizontalHeader()->setStretchLastSection( true );
  ui->tableView->setAlternatingRowColors( true );
  ui->treeView->setAlternatingRowColors( true );
  connect( ui->treeView, &QTreeView::clicked, this, &MainWindow::treeClicked );
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::openFile()
{
  auto fileName = QFileDialog::getOpenFileName( this, tr("Open CSV"), QDir::homePath(), tr("CSV Files (*.csv)") );
  if ( fileName.isNull() || fileName.isEmpty() ) return;

  parse( fileName );
}

void MainWindow::dragEnterEvent( QDragEnterEvent* event )
{
  if ( event->mimeData()->hasUrls() ) event->acceptProposedAction();
  else qWarning(MAIN_WINDOW) << "Ignoring drop for non-url type";
}

void MainWindow::dropEvent( QDropEvent* event )
{
  for ( int i = 0; i < event->mimeData()->urls().size(); ++i )
  {
    const auto fileName = event->mimeData()->urls().at( i ).toLocalFile();
    qInfo(MAIN_WINDOW) << "Dropped file:" << fileName;

    const auto file = QFileInfo( fileName );
    if ( file.isFile() && file.suffix() == "csv" )
    {
      parse( file.absoluteFilePath() );
    }
    else
    {
      qWarning(MAIN_WINDOW) << "Unsupported file type:" << file.absoluteFilePath();
    }
  }
}

void MainWindow::treeClicked( const QModelIndex& index )
{
  if ( !index.isValid() ) return;

  auto* tm = dynamic_cast<model::TreeModel*>( ui->treeView->model() );
  if ( !tm )
  {
    qCritical(MAIN_WINDOW) << "Tree model is not a model::TreeModel instance.";
    return;
  }

  const auto* node = tm->nodeFor( index );
  auto* model = new QStandardItemModel( this );
  model->setHorizontalHeaderLabels( { "Key", "Value" } );

  int i = 0;
  model->setItem( i, 0, new QStandardItem( "id" ) );
  model->setItem( i, 1, new QStandardItem( node->id ) );
  ++i;

  if ( !node->file.isEmpty() )
  {
    model->setItem( i, 0, new QStandardItem( "file" ) );
    model->setItem( i, 1, new QStandardItem( node->file ) );
    ++i;
  }
  if ( !node->function.isEmpty() )
  {
    model->setItem( i, 0, new QStandardItem( "function" ) );
    model->setItem( i, 1, new QStandardItem( node->function ) );
    ++i;
  }
  if ( node->line > 0 )
  {
    model->setItem( i, 0, new QStandardItem( "line" ) );
    model->setItem( i, 1, new QStandardItem( QString( "%1" ).arg( node->line ) ) );
    ++i;
  }
  if ( node->timestamp.time_since_epoch().count() > 0 )
  {
    model->setItem( i, 0, new QStandardItem( "timestamp" ) );
    model->setItem( i, 1, new QStandardItem( QString::fromStdString( date::isoDateMicros( node->timestamp ) ) ) );
    ++i;
  }
  if ( node->duration > 0 )
  {
    model->setItem( i, 0, new QStandardItem( "duration" ) );
    model->setItem( i, 1, new QStandardItem( QString( "%1" ).arg( node->duration ) ) );
    ++i;
  }

  for ( const auto& [key, value] : node->keyValues() )
  {
    model->setItem( i, 0, new QStandardItem( key ) );
    model->setItem( i, 1, new QStandardItem( value ) );
    ++i;
  }
  ui->tableView->setModel( model );
}


void MainWindow::parse( const QString& file )
{
  auto parser = new model::Parser( file );
  auto thread = new QThread;
  parser->moveToThread( thread );

  connect( parser, &model::Parser::progress, this, [this]( std::size_t total )
  {
    statusBar()->showMessage( QString( "Parsed %1 rows" ).arg( total ) );
  } );
  connect( parser, &model::Parser::finished, this, [this]( model::Node* root )
  {
    qInfo(MAIN_WINDOW) << "Setting tree model";
    ui->treeView->setModel( new model::TreeModel( root, this ) );
  } );

  connect( thread, &QThread::started, parser, &model::Parser::parse );
  connect( thread, &QThread::finished, parser, &QObject::deleteLater );
  connect( thread, &QThread::finished, thread, &QThread::deleteLater );

  thread->start();
}
