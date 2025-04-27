//
// Created by Rakesh on 25/04/2025.
//

#pragma once

#include <Wt/WContainerWidget.h>

namespace Wt
{
  class WDialog;
  class WModelIndex;
  class WMouseEvent;
  class WPushButton;
  class WTableView;
  class WText;
  class WTextArea;
}

namespace spt::model
{
  struct TableModel;
}

namespace spt::apm
{
  struct ListView : Wt::WContainerWidget
  {
    ListView();

    void execute();

  private:
    using Wt::WContainerWidget::clear;
    using Wt::WContainerWidget::removeWidget;

    void showTree( Wt::WModelIndex index, Wt::WMouseEvent event );

    Wt::WTextArea* query{ nullptr };
    Wt::WTableView* table{ nullptr };
    Wt::WText* error{ nullptr };
    Wt::WPushButton* submit{ nullptr };
    Wt::WDialog* dialog{ nullptr };

    std::shared_ptr<model::TableModel> model{ nullptr };
  };
}
