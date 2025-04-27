//
// Created by Rakesh on 23/12/2023.
//

#include "application.hpp"
#include "listview.hpp"
#include "page.hpp"
#include "treeview.hpp"

#include <Wt/WCheckBox.h>
#include <log/NanoLog.hpp>
#include <Wt/WAnchor.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>

using spt::apm::Page;
using std::operator""s;
using std::operator""sv;

Page::Page() : Wt::WContainerWidget()
{
  auto t = addWidget( std::make_unique<Wt::WTemplate>( tr( "apm.page.navbar" ) ) );

  t->bindWidget( "list", std::make_unique<Wt::WAnchor>(
      Wt::WLink( Wt::LinkType::InternalPath, "/list" ), tr( "apm.page.navbar.menu.list" ) ) )->setObjectName( "List" );
  t->bindWidget( "tree", std::make_unique<Wt::WAnchor>(
      Wt::WLink( Wt::LinkType::InternalPath, "/tree" ), tr( "apm.page.navbar.menu.tree" ) ) )->setObjectName( "Tree View" );
  t->bindWidget( "logout", std::make_unique<Wt::WAnchor>(
      Wt::WLink( Wt::LinkType::InternalPath, "/logout" ), tr( "apm.page.navbar.menu.logout" ) ) )->setObjectName( "Logout" );
}

void Page::display()
{
  auto app = Application::instance();
  auto lastPath = app->getLastPath();
  if ( lastPath.empty() || lastPath == "/" || lastPath == "/a" ) lastPath = "/list"s;

  Application::instance()->log( "info" ) << __FILE__ << " " << __func__ << " " << __LINE__ <<  " Displaying widget for path " << lastPath;
  app->resetEntryPath();

  if ( count() > 1 ) removeWidget( widget( 1 ) );

  if ( lastPath.ends_with( "/list"sv ) ) displayListView();
  else if ( lastPath.ends_with( "/tree"sv ) ) displayTreeView();
  else
  {
    Application::instance()->log( "error" ) << __FILE__ << " " << __func__ << " " << __LINE__ <<  " Unknown path " << lastPath;
    addChild( std::make_unique<Wt::WText>( std::format( "Unknown path {}", lastPath ) ) );
  }
}

void Page::displayListView()
{
  if ( count() > 1 ) removeWidget( widget( 1 ) );
  Application::instance()->log( "info" ) << __FILE__ << " " << __func__ << " " << __LINE__ <<  " Display latest parent APM records";
  addWidget( std::make_unique<ListView>() );
}

void Page::displayTreeView()
{
  if ( count() > 1 ) removeWidget( widget( 1 ) );
  Application::instance()->log( "info" ) << __FILE__ << " " << __func__ << " " << __LINE__ <<  " Display latest APM records as a tree";
  addWidget( std::make_unique<TreeView>() );
}