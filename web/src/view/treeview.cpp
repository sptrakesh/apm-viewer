//
// Created by Rakesh on 26/04/2025.
//

#include "application.hpp"
#include "treeview.hpp"
#include "model/propertiesmodel.hpp"
#include "model/treemodel.hpp"
#include "tsdb/questdb.hpp"

#include <Wt/WIntValidator.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WTableView.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>
#include <Wt/WTreeView.h>
#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
#include <fmt/ranges.h>

#include "model/tablemodel.hpp"

using spt::apm::TreeView;
using std::operator""sv;

namespace
{
  namespace ptv
  {
    std::expected<std::vector<std::string>, std::string> latestIds( int rows )
    {
      using O = std::expected<std::vector<std::string>, std::string>;

      auto result = spt::tsdb::execute( fmt::format( R"(select id
from webapm
where type is null
order by timestamp desc
limit {})", rows ) );

      if ( !result.has_value() ) return O{ std::unexpect, result.error() };

      auto ids = O{ std::in_place };
      ids->reserve( result->size() );
      for ( const auto& row : *result )
      {
        ids->emplace_back( row.at( 0 ).as<std::string>() );
      }

      return ids;
    }

    std::expected<std::shared_ptr<spt::model::TreeModel>, std::string> model( int rows )
    {
      using O = std::expected<std::shared_ptr<spt::model::TreeModel>, std::string>;
      auto ids = latestIds( rows );
      if ( !ids.has_value() ) return O{ std::unexpect, std::move( ids.error() ) };

      auto query = fmt::format( R"(select *
from webapm
where id in ({:n})
order by timestamp)", ids.value() );
      boost::algorithm::replace_all( query, "\"", "'" );

      auto result = spt::tsdb::execute( query );
      if ( !result.has_value() ) return O{ std::unexpect, std::move( result.error() ) };
      return O{ std::in_place, std::make_shared<spt::model::TreeModel>( result.value() ) };
    }
  }
}

TreeView::TreeView()
{
  Application::instance()->setTitle( "Tree"sv );
  Application::instance()->setPath( "/tree" );
  auto t = addWidget( std::make_unique<Wt::WTemplate>( tr( "apm.page.tree" ) ) );

  rows = t->bindWidget( "rows", std::make_unique<Wt::WLineEdit>() );
  rows->setObjectName( "tree-rows-field" );
  rows->setValidator( std::make_shared<Wt::WIntValidator>( 1, 1000 ) );
  rows->setText( "10" );
  rows->setFocus();

  submit = t->bindWidget( "submit", std::make_unique<Wt::WPushButton>( "Submit" ) );
  submit->setStyleClass( "button__icon fas fa-chevron-right" );
  submit->setObjectName( "auth-loginview-submit" );
  submit->clicked().connect( this, &TreeView::execute );

  tree = t->bindWidget( "tree", std::make_unique<Wt::WTreeView>() );
  tree->setRootIsDecorated( true );
  tree->setAlternatingRowColors( true );
  tree->setColumnResizeEnabled( true );
  tree->setSortingEnabled( false );
  tree->setSelectable( true );
  tree->setSelectionBehavior( Wt::SelectionBehavior::Rows );
  tree->setSelectionMode( Wt::SelectionMode::Single );

  table = t->bindWidget( "table", std::make_unique<Wt::WTableView>() );
  table->setObjectName( "apm-list-table-field" );
  table->setAlternatingRowColors( true );
  table->setSortingEnabled( false );
  table->setColumnResizeEnabled( true );

  error = t->bindWidget( "error", std::make_unique<Wt::WText>() );
  error->setStyleClass( "login__input" );
  error->setObjectName( "apm-error-field" );
  error->setHidden( true );

  rows->enterPressed().connect( [this] { submit->clicked().emit( Wt::WMouseEvent() ); } );
  tree->selectionChanged().connect( [this] { displayProperties(); } );

  execute();
}

void TreeView::execute()
{
  error->setHidden( true );
  auto m = ptv::model( boost::lexical_cast<int>( rows->text().toUTF8() ) );
  if ( !m.has_value() )
  {
    error->setText( m.error() );
    error->setHidden( false );
    return;
  }

  tree->setModel( m.value() );
}

void TreeView::displayProperties()
{
  if ( tree->selectedIndexes().empty() ) return;
  auto* node = static_cast<model::Node*>( tree->selectedIndexes().begin()->internalPointer() );
  auto model = std::make_shared<model::PropertiesModel>( node );
  table->setModel( model );
}