#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtCore/QItemSelection>
#include <QtWidgets/QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

public slots:
  void openFile();

protected:
  void dragEnterEvent(QDragEnterEvent* event) override;
  void dropEvent(QDropEvent* event) override;

private slots:
  void treeClicked( const QModelIndex& index );

private:
  void parse( const QString& file );

  Ui::MainWindow* ui;
};
#endif // MAINWINDOW_HPP
