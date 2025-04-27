//
// Created by Rakesh on 21/12/2023.
//

#pragma once

#include <Wt/WContainerWidget.h>

namespace Wt
{
  class WComboBox;
  class WLineEdit;
  class WPushButton;
  class WText;
}

namespace spt::apm
{
  struct AuthWidget;

  struct LoginViewModel
  {
    std::string username;
    std::string password;
  };

  struct LoginView : Wt::WContainerWidget
  {
    explicit LoginView( AuthWidget* widget );

    LoginViewModel model();

    void focusUsername();
    void focusPassword();
    void hideError();
    void enableSubmit( bool );
    void showError( const std::string& text );

  private:
    Wt::WLineEdit* username{ nullptr };
    Wt::WLineEdit* password{ nullptr };
    Wt::WText* error{ nullptr };
    Wt::WPushButton* submit{ nullptr };
  };
}
