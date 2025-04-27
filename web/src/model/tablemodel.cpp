//
// Created by Rakesh on 25/04/2025.
//

#include "tablemodel.hpp"
#include "tsdb/pgtypes.hpp"
#include "view/application.hpp"

#include <boost/algorithm/string/replace.hpp>
#include <fmt/ranges.h>
#include <range/v3/algorithm/find.hpp>
#include <Wt/WDateTime.h>

using spt::model::TableModel;

TableModel::TableModel( pqxx::result res ) : result( std::move( res ) )
{
  for ( const auto& row : result )
  {
    for ( const auto& column : row )
    {
      if ( column.is_null() ) continue;
      auto name = std::string{ column.name() };
      boost::replace_all( name, "\"", "" );
      if ( auto iter = ranges::find( columns, name ); iter == ranges::end( columns ) ) columns.emplace_back( column.name() );
    }
  }

  apm::Application::instance()->log( "info" ) << __FILE__ << " " << __func__ << " " << __LINE__ <<
    fmt::format( " Rows: {}, Columns: {:n}", result.size(), columns );
}

std::any TableModel::data( const Wt::WModelIndex& index, Wt::ItemDataRole role ) const
{
  if ( role != Wt::ItemDataRole::Display
      && role != Wt::ItemDataRole::Edit
      && role != Wt::ItemDataRole::ToolTip ) return {};

  if ( index.row() >= result.size() || index.column() >= columns.size() ) return {};
  auto it = result.begin();
  std::advance( it, index.row() );
  const auto& field = it->at( columns.at( index.column() ) );

  if ( tsdb::isTimestamp( field ) )
  {
    auto ts = tsdb::timestamp( field );
    return ts ? Wt::WDateTime{ *ts } : Wt::WDateTime::currentDateTime();
  }

  if ( tsdb::isString( field ) )
  {
    auto str = tsdb::string( field );
    return str ? Wt::WString{ *str } : Wt::WString{};
  }

  return tsdb::number( field ).value_or( 0.0 );
}

std::any TableModel::headerData( int section, Wt::Orientation orientation, Wt::ItemDataRole role ) const
{
  if ( orientation != Wt::Orientation::Horizontal ) return WAbstractTableModel::headerData( section, orientation, role );

  if ( role == Wt::ItemDataRole::Link ) return WAbstractItemModel::headerData( section, orientation, role );

  if ( role != Wt::ItemDataRole::Display && role != Wt::ItemDataRole::Edit ) return {};

  return Wt::WString( columns.at( section ) );
}