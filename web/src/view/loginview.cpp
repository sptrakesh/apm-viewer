//
// Created by Rakesh on 21/12/2023.
//

#include "application.hpp"
#include "authwidget.hpp"
#include "loginview.hpp"

#include <fmt/format.h>
#include <Wt/WComboBox.h>
#include <Wt/WEvent.h>
#include <Wt/WLabel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>

using spt::apm::LoginView;
using spt::apm::LoginViewModel;
using std::operator""sv;
using std::operator""sv;

LoginView::LoginView( AuthWidget* widget ) : Wt::WContainerWidget()
{
  Application::instance()->setTitle( "Login"sv );
  auto t = addWidget( std::make_unique<Wt::WTemplate>( tr( "apm.auth.login" ) ) );
  username = t->bindWidget( "user-name", std::make_unique<Wt::WLineEdit>() );
  username->setStyleClass( "login__input" );
  username->setPlaceholderText( "username" );
  username->setAutoComplete( true );
  username->setFocus();
  username->setObjectName( "auth-loginview-username-field" );

  password = t->bindWidget( "password", std::make_unique<Wt::WLineEdit>() );
  password->setStyleClass( "login__input" );
  password->setEchoMode( Wt::EchoMode::Password );
  password->setPlaceholderText( "password" );
  password->setAutoComplete( true );
  password->setObjectName( "auth-loginview-password-field" );

  submit = t->bindWidget( "login", std::make_unique<Wt::WPushButton>( "Submit" ) );
  submit->setStyleClass( "button__icon fas fa-chevron-right" );
  submit->setObjectName( "auth-loginview-submit" );
  submit->clicked().connect( widget, &AuthWidget::submit );

  error = t->bindWidget( "error", std::make_unique<Wt::WText>() );
  error->setStyleClass( "login__input" );
  error->setObjectName( "auth-loginview-error-field" );
  error->setHidden( true );

  username->enterPressed().connect( [this] { password->setFocus(); } );
  password->enterPressed().connect( [this] { submit->clicked().emit( Wt::WMouseEvent() ); } );
}

LoginViewModel LoginView::model()
{
  return { .username = username->text().toUTF8(), .password = password->text().toUTF8() };
}

void LoginView::focusUsername() { username->setFocus(); }
void LoginView::focusPassword() { password->setFocus(); }
void LoginView::hideError() { error->setHidden( true ); }
void LoginView::enableSubmit( bool flag ) { submit->setEnabled( flag ); }

void LoginView::showError( const std::string& text )
{
  error->setText( text );
  error->setHidden( false );
}
