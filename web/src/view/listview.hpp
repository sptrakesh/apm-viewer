//
// Created by Rakesh on 25/04/2025.
//

#pragma once

#include <Wt/WContainerWidget.h>

namespace Wt
{
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

    Wt::WTextArea* query{ nullptr };
    Wt::WTableView* table{ nullptr };
    Wt::WText* error{ nullptr };
    Wt::WPushButton* submit{ nullptr };

    std::shared_ptr<model::TableModel> model{ nullptr };
  };
}
