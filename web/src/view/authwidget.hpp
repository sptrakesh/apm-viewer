//
// Created by Rakesh on 19/12/2023.
//

#pragma once

#include <Wt/WContainerWidget.h>
#include <Wt/WSignal.h>

namespace spt::apm
{
  struct LoginView;

  struct AuthWidget : Wt::WContainerWidget
  {
    AuthWidget();

    void submit();
    inline Wt::Signal<bool>& loggedIn() { return _loggedIn; }

  private:
    LoginView* loginView{ nullptr };
    Wt::Signal<bool> _loggedIn{};
  };
}