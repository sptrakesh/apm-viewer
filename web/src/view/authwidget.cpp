//
// Created by Rakesh on 19/12/2023.
//

#include "authwidget.hpp"
#include "loginview.hpp"
#include "application.hpp"

#include <cstdlib>
#include <format>
#include <Wt/Http/Cookie.h>
#include <Wt/WServer.h>

using spt::apm::AuthWidget;

namespace
{
  namespace pauth
  {
    std::string environmentValue( const char* key )
    {
      if ( const char* value = std::getenv( key ); value ) return value;
      return {};
    }

    std::string username()
    {
      static const std::string defaultUsername{ "apmuser" };
      static const std::string env = environmentValue( "APM_USERNAME" );
      return env.empty() ? defaultUsername : env;
    }

    std::string password()
    {
      static const std::string defaultPassword{ "GRBZvE8Kq1kHzK9d1dWJ+UU9ro7cVYBxR9DNxI1GGSw=" };
      static const std::string env = environmentValue( "APM_PASSWORD" );
      return env.empty() ? defaultPassword : env;
    }
  }
}

AuthWidget::AuthWidget() : Wt::WContainerWidget()
{
  loginView = addWidget( std::make_unique<LoginView>( this ) );
}

void AuthWidget::submit()
{
  const auto model = loginView->model();

  if ( model.username.empty() )
  {
    loginView->showError( "Please enter your username!" );
    loginView->focusUsername();
    return;
  }

  if ( model.password.empty() )
  {
    loginView->showError( "Please enter your password!" );
    loginView->focusPassword();
    return;
  }

  loginView->enableSubmit( false );
  loginView->hideError();

  if ( model.username != pauth::username() || model.password != pauth::password() )
  {
    loginView->showError( "Invalid credentials" );
    loginView->enableSubmit( true );
    return;
  }

  removeWidget( loginView );
  loginView = nullptr;
  _loggedIn.emit( true );
}