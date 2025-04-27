//
// Created by Rakesh on 23/12/2023.
//

#pragma once

#include <Wt/WContainerWidget.h>

namespace spt::apm
{
  struct Page : Wt::WContainerWidget
  {
    Page();

    void display();

  private:
    void displayListView();
    void displayTreeView();

    using Wt::WContainerWidget::clear;
    using Wt::WContainerWidget::removeWidget;
  };
}
