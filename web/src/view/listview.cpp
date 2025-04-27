//
// Created by Rakesh on 25/04/2025.
//

#include "application.hpp"
#include "listview.hpp"
#include "treeview.hpp"
#include "model/tablemodel.hpp"
#include "tsdb/questdb.hpp"

#include <Wt/WDialog.h>
#include <Wt/WEvent.h>
#include <Wt/WModelIndex.h>
#include <Wt/WPushButton.h>
#include <Wt/WTableView.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>
#include <Wt/WTextArea.h>
#include <Wt/WTreeView.h>

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
  query->setText( std::format( R"(
select *
from {}
where type is null
order by timestamp desc
limit 100
)", tsdb::table() ) );

  submit = t->bindWidget( "submit", std::make_unique<Wt::WPushButton>( "Submit" ) );
  submit->setStyleClass( "button-10" );
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

  table->doubleClicked().connect( this, &ListView::showTree );
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

void ListView::showTree( Wt::WModelIndex index, Wt::WMouseEvent )
{
  auto id = model->getId( index );
  if ( id.empty() ) return;

  if ( dialog )
  {
    dialog->finished();
  }

  dialog = addChild( std::make_unique<Wt::WDialog>( "Full APM Details" ) );
  dialog->contents()->addWidget( std::make_unique<TreeView>( id ) );
  dialog->resize( "100%", "100%" );
  dialog->finished().connect( [this] { removeChild( dialog ); dialog = nullptr; } );

  auto* btn = dialog->footer()->addWidget( std::make_unique<Wt::WPushButton>( "Close" ) );
  btn->setStyleClass( "button-10" );
  btn->clicked().connect( dialog, &Wt::WDialog::accept );

  dialog->show();
}