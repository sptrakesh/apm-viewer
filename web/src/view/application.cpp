//
// Created by Rakesh on 04/06/2023.
//

#include "application.hpp"
#include "page.hpp"
#include "authwidget.hpp"

#include <boost/url/parse.hpp>
#include <boost/url/static_url.hpp>
#include <fmt/format.h>
#include <log/NanoLog.hpp>
#include <Wt/WEnvironment.h>
#include <Wt/WLength.h>
#include <Wt/WMessageBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WServer.h>
#include <Wt/WString.h>
#include <Wt/Http/Cookie.h>

using spt::apm::Application;
using std::operator""s;
using std::operator""sv;

Application::Application( const Wt::WEnvironment& wenv ) : WApplication( wenv )
{
  addMetaHeader( Wt::MetaHeaderType::Meta, "width=device-width", "initial-scale=1", "maximum-scale=1" );
  messageResourceBundle().use( fmt::format( "{}templates/auth", appRoot() ) );
  messageResourceBundle().use( fmt::format( "{}templates/page", appRoot() ) );
  messageResourceBundle().use( fmt::format( "{}templates/list", appRoot() ) );
  messageResourceBundle().use( fmt::format( "{}templates/tree", appRoot() ) );
  useStyleSheet( "css/style.css" );

  setTitle();
  entryPath = wenv.internalPath();
  log( "info" ) << __FILE__ << " " << __func__ << " " << __LINE__ << " Entry path: " << entryPath;

  internalPathChanged().connect( this, &Application::pathChangeListener );

  const auto cn = cookieName();
  if ( auto* cookie = wenv.getCookie( cn ); cookie )
  {
    log( "debug" ) << __FILE__ << " " << __func__ << " " << __LINE__ << " Cookie value " << *cookie;
    display( *cookie, cn );
    return;
  }
  log( "debug" ) << __FILE__ << " " << __func__ << " " << __LINE__ <<  " No cookie set";

  root()->clear();
  displayAuth();
}

Application* Application::instance()
{
  return dynamic_cast<Application*>( Wt::WApplication::instance() );
}

void Application::setPath( const std::string& path )
{
  lastPath = path;
  setInternalPath( path );
}

void Application::setTitle( std::string_view title )
{
  if ( title.empty() ) Wt::WApplication::setTitle( "APM Application:" );
  else Wt::WApplication::setTitle( fmt::format( "APM Application: {}", title ) );
}

void Application::pathChangeListener( const std::string& path )
{
  log( "info" ) << __FILE__ << " " << __func__ << " " << __LINE__ << " Handling path change to " << path;

  if ( path.ends_with( "/logout" ) ) return logout();

  lastPath = path;

  if ( !page )
  {
    log( "crit" ) << __FILE__ << " " << __func__ << " " << __LINE__ <<  " No page instance to handle path change";
    root()->clear();
    root()->addWidget( std::make_unique<AuthWidget>() );
    return;
  }

  page->display();
}

std::string Application::cookieName()
{
#ifdef __APPLE__
  return "spt-apm-auth"s;
#else
  return "__Host-spt-apm-auth"s;
#endif
}

void Application::displayAuth()
{
  root()->clear();
  auto widget = root()->addWidget( std::make_unique<AuthWidget>() );
  widget->loggedIn().connect( this, &Application::handleLogin );
}

void Application::display( std::string_view cookie, const std::string& cookieName )
{
  if ( !cookie.empty() )
  {
    root()->clear();
    return route();
  }

  displayAuth();

  auto ck = Wt::Http::Cookie{ cookieName };
#ifdef __APPLE__
  ck.setSecure( false );
#else
  ck.setSecure( true );
#endif
  ck.setHttpOnly( true );
  ck.setPath( "/"s );
  removeCookie( ck );
}

void Application::route( bool fromLogout )
{
  const auto ip = entryPath.empty() ? internalPath() : entryPath;
  const auto path = ip.empty() || ip == "/a" ? "/list"s : ip;

  if ( !fromLogout && path.ends_with( "/logout" ) ) return logout();

  displayPage();
}

void Application::logout()
{
  log( "info" ) << __FILE__ << " " << __func__ << " " << __LINE__ <<  " Logging out. Entry path " << lastPath;
  clearSession();
  setInternalPath( "/a" );
  displayAuth();
}

void Application::handleLogin()
{
  createCookie();
  displayPage();
}

void Application::displayPage()
{
  if ( page ) return page->display();

  root()->clear();
  page = root()->addWidget( std::make_unique<Page>() );
  page->display();
}

void Application::createCookie()
{
  auto cookie = Wt::Http::Cookie( cookieName(),
      "0t0VMLvsK54hzE1AsZ5fkUPV8H9AVx+48K/DTVs4oA0="s,
#ifdef __APPLE__
      std::chrono::hours{ 8 } );
  cookie.setSecure( false );
  cookie.setSameSite( Wt::Http::Cookie::SameSite::Lax );
#else
      std::chrono::hours{ 24 } );
  cookie.setSecure( true );
  cookie.setSameSite( Wt::Http::Cookie::SameSite::Strict );
#endif
  cookie.setHttpOnly( true );
  cookie.setPath( "/"s );
  setCookie( cookie );
}

void Application::clearSession()
{
  auto cookie = Wt::Http::Cookie{ cookieName() };
  cookie.setSecure( true );
  cookie.setHttpOnly( true );
  cookie.setPath( "/"s );
  removeCookie( cookie );
}
