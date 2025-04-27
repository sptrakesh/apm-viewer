//
// Created by Rakesh on 25/04/2025.
//

#include "application.hpp"
#include "listview.hpp"
#include "model/tablemodel.hpp"
#include "tsdb/questdb.hpp"

#include <Wt/WPushButton.h>
#include <Wt/WTableView.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>
#include <Wt/WTextArea.h>

using spt::apm::ListView;
using std::operator""sv;

ListView::ListView()
{
  Application::instance()->setTitle( "List"sv );
  Application::instance()->setPath( "/list" );
  auto t = addWidget( std::make_unique<Wt::WTemplate>( tr( "apm.page.list" ) ) );

  query = t->bindWidget( "query", std::make_unique<Wt::WTextArea>() );
  query->setFocus();
  query->setObjectName( "apm-list-query-field" );
  query->setRows( 5 );
  query->setText( R"(
select *
from webapm
where type is null
order by timestamp desc
limit 100
)" );

  submit = t->bindWidget( "submit", std::make_unique<Wt::WPushButton>( "Submit" ) );
  submit->setStyleClass( "button__icon fas fa-chevron-right" );
  submit->setObjectName( "auth-loginview-submit" );
  submit->clicked().connect( this, &ListView::execute );

  table = t->bindWidget( "table", std::make_unique<Wt::WTableView>() );
  table->setObjectName( "apm-list-table-field" );
  table->setAlternatingRowColors( true );
  table->setSortingEnabled( false );
  table->setColumnResizeEnabled( true );

  error = t->bindWidget( "error", std::make_unique<Wt::WText>() );
  error->setStyleClass( "login__input" );
  error->setObjectName( "apm-error-field" );
  error->setHidden( true );
}

void ListView::execute()
{
  error->setHidden( true );
  if ( query->text().empty() ) return;

  auto result = tsdb::execute( query->text().toUTF8() );
  if ( !result.has_value() )
  {
    error->setText( result.error() );
    error->setHidden( false );
    return;
  }

  model = std::make_shared<model::TableModel>( std::move( result.value() ) );
  auto idx = model->index( 0, 0 );
  for ( int i = 0; i < model->columnCount( idx ); ++i ) table->setColumnAlignment( i, Wt::AlignmentFlag::Left | Wt::AlignmentFlag::Middle );
  table->setModel( model );
}
