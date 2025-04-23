# pragma once

#include <QtWidgets/QWidget>

namespace view
{
  namespace Ui { class LogViewer; }

  class LogViewer : public QWidget
  {
    Q_OBJECT

  public:
    explicit LogViewer(QWidget* parent = nullptr);
    ~LogViewer();

  public slots:
    void refresh();

  private:
    QString addHtmlColor(const QString& text) const;

    Ui::LogViewer* ui;
  };

} // namespace view
