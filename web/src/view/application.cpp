//
// Created by Rakesh on 04/06/2023.
//

#include "application.hpp"
#include "page.hpp"
#include "authwidget.hpp"
#include "util/base64.hpp"
#include "util/date.hpp"

#include <Wt/Http/Cookie.h>
#include <Wt/WEnvironment.h>
#include <Wt/WPushButton.h>
#include <Wt/WServer.h>
#include <Wt/WString.h>
#include <boost/json/parse.hpp>
#include <boost/json/serialize.hpp>
#include <filesystem>
#include <fmt/format.h>
#include <fmt/os.h>
#include <fstream>

using spt::apm::Application;
using std::operator""s;
using std::operator""sv;

namespace
{
  namespace papp
  {
    bool secureCookie()
    {
      if ( const char* value = std::getenv( "SECURE_COOKIE" ); value ) return "true"sv == value;
      return false;
    }

    std::string encrypt( std::string_view value )
    {
      static const std::string key{ "0t0VMLvsK54hzE1AsZ5fkUPV8H9AVx+48K/DTVs4oA0=" };
      auto result = std::string{ value };

      for ( int i = 0; i < value.size(); i++ )
      {
        result[i] = value[i] ^ key[i % (key.size() / sizeof(char))];
      }

      return result;
    }

    struct SessionManager
    {
      using Time = std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>;

      static SessionManager& instance()
      {
        static SessionManager holder;
        return holder;
      }

      SessionManager(const SessionManager&) = delete;
      SessionManager& operator=(const SessionManager&) = delete;

      [[nodiscard]] std::string create()
      {
        auto time = Time{ std::chrono::duration_cast<std::chrono::microseconds>( std::chrono::system_clock::now().time_since_epoch() ) };
        auto id = spt::util::isoDateMicros( time );
        sessions.try_emplace( id, time );

        save();
        return base64::to_base64( encrypt( id ) );
      }

      [[nodiscard]] bool validate( std::string_view value )
      {
        auto decoded = base64::from_base64( std::string{ value } );
        if ( decoded.empty() ) return false;
        auto iter = sessions.find( encrypt( decoded ) );
        if ( iter == sessions.end() ) return false;
        auto time = Time{ std::chrono::duration_cast<std::chrono::microseconds>( std::chrono::system_clock::now().time_since_epoch() ) };
        return time > iter->second;
      }

      void remove( std::string_view value )
      {
        auto decoded = base64::from_base64( std::string{ value } );
        if ( decoded.empty() )
        {
          std::cerr << "Empty base64 decoded value " << value << std::endl;
          return;
        }

        auto iter = sessions.find( encrypt( decoded ) );
        if ( iter == sessions.end() )
        {
          std::cerr << "Session not found from value " << encrypt( decoded ) << std::endl;
          return;
        }

        sessions.erase( iter );
        save();
      }

    private:
      SessionManager()
      {
        std::error_code ec;
        auto p = std::filesystem::path{ file };
        if ( std::filesystem::exists( p, ec ) && is_regular_file( p ) )
        {
          auto size = std::filesystem::file_size( p, ec );
          auto body = std::string{};
          body.resize( size, '\0' );
          auto stream = std::ifstream( p, std::ios::in | std::ios::binary );
          stream.exceptions( std::ios_base::badbit );
          if ( !stream ) return;
          stream.read( &body[0], static_cast<std::streamsize>( size ) );

          auto parsed = boost::json::parse( body, ec );
          if ( ec )
          {
            std::cerr << "Error parsing session file: " << ec.message() << std::endl;
            return;
          }

          if ( !parsed.is_array() )
          {
            std::cerr << "Session data not array: " << ec.message() << std::endl;
            return;
          }

          const auto& arr = parsed.as_array();
          for ( const auto& obj : arr )
          {
            if ( !obj.is_object() )
            {
              std::cerr << "Session data not object\n";
              continue;
            }

            const auto& object = obj.as_object();
            if ( !object.contains( "id" ) || !object.contains( "value" ) )
            {
              std::cerr << "Session data missing id or value\n";
              continue;
            }

            auto id = object.at( "id" );
            if ( !id.is_string() )
            {
              std::cerr << "Session id not string\n";
              continue;
            }

            auto us = object.at( "value" );
            if ( !us.is_int64() )
            {
              std::cerr << "Session value not int64\n";
              continue;
            }

            sessions.try_emplace( std::string{ id.as_string() }, Time{ std::chrono::microseconds{ us.as_int64() } } );
          }
        }
      }

      void save() const
      {
        auto json = boost::json::array{};
        for ( const auto& [key, value] : sessions )
        {
          json.emplace_back( boost::json::object{ { "id", key }, { "value", std::chrono::duration_cast<std::chrono::microseconds>( value.time_since_epoch() ).count() } } );
        }
        fmt::output_file( file ).print( "{}", boost::json::serialize( json ) );
      }

      std::map<std::string, std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds>, std::less<>> sessions;
      const std::string file{ "/tmp/apm-sessions.json" };
    };
  }
}

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
  if ( papp::secureCookie() ) return "__Host-spt-apm-auth"s;
  return "spt-apm-auth"s;
}

void Application::displayAuth()
{
  root()->clear();
  auto widget = root()->addWidget( std::make_unique<AuthWidget>() );
  widget->loggedIn().connect( this, &Application::handleLogin );
}

void Application::display( std::string_view cookie, const std::string& cookieName )
{
  if ( !cookie.empty() && papp::SessionManager::instance().validate( cookie ) )
  {
    root()->clear();
    return route();
  }

  displayAuth();

  auto ck = Wt::Http::Cookie{ cookieName };
  ck.setSecure( papp::secureCookie() );
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
  setInternalPath( "" );
  page = nullptr;
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
  cookieValue = papp::SessionManager::instance().create();
  auto cookie = Wt::Http::Cookie( cookieName(), cookieValue,
#ifdef __APPLE__
      std::chrono::hours{ 8 } );
#else
      std::chrono::hours{ 24 } );
#endif

  if ( papp::secureCookie() )
  {
    cookie.setSecure( true );
    cookie.setSameSite( Wt::Http::Cookie::SameSite::Strict );
  }
  else
  {
    cookie.setSecure( false );
    cookie.setSameSite( Wt::Http::Cookie::SameSite::Lax );
  }

  cookie.setHttpOnly( true );
  cookie.setPath( "/"s );
  setCookie( cookie );
}

void Application::clearSession()
{
  auto cookie = Wt::Http::Cookie{ cookieName() };
  cookie.setSecure( papp::secureCookie() );
  cookie.setHttpOnly( true );
  cookie.setPath( "/"s );
  removeCookie( cookie );
  papp::SessionManager::instance().remove( cookieValue );
  cookieValue.clear();
}
