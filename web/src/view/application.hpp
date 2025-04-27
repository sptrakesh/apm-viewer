//
// Created by Rakesh on 04/06/2023.
//

#pragma once

#include <string_view>
#include <Wt/WApplication.h>

namespace Wt
{
  class WString;
  class WWidget;
}

namespace spt::apm
{
  struct Page;

  struct Application : Wt::WApplication
  {
    explicit Application( const Wt::WEnvironment& wenv );

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    static Application* instance();

    void setPath( const std::string &path );
    void setTitle( std::string_view title = {} );
    void pathChangeListener( const std::string& path );

    [[nodiscard]] inline std::string getEntryPath() const { return entryPath; }
    void resetEntryPath() { entryPath = ""; }
    [[nodiscard]] inline std::string getLastPath() const { return lastPath; }

    void handleLogin();
    void displayPage();
    void clearSession();

  private:
    using Wt::WApplication::setTitle;
    using Wt::WApplication::setInternalPath;
    using Wt::WApplication::root;

    std::string cookieName();
    void createCookie();
    void displayAuth();
    void display( std::string_view cookie, const std::string& cookieName );
    void route( bool fromLogout = false );
    void logout();

    std::string entryPath{};
    std::string lastPath{};
    Page* page{ nullptr };
  };
}