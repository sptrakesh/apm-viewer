//
// Created by Rakesh on 26/04/2025.
//

#pragma once

#include <Wt/WContainerWidget.h>

namespace Wt
{
  class WLineEdit;
  class WPushButton;
  class WTableView;
  class WText;
  class WTreeView;
}

namespace spt::model
{
  struct TreeModel;
}

namespace spt::apm
{
  struct TreeView : Wt::WContainerWidget
  {
    TreeView();
    explicit TreeView( std::string_view id );

    void execute();

  private:
    using Wt::WContainerWidget::clear;
    using Wt::WContainerWidget::removeWidget;

    void init( bool full );
    void displayProperties();

    Wt::WLineEdit* rows{ nullptr };
    Wt::WPushButton* submit{ nullptr };
    Wt::WTreeView* tree{ nullptr };
    Wt::WTableView* table{ nullptr };
    Wt::WText* error{ nullptr };

    std::shared_ptr<model::TreeModel> model{ nullptr };
  };
}
