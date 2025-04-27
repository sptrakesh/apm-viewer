//
// Created by Rakesh on 26/04/2025.
//

#include "propertiesmodel.hpp"
#include "util/date.hpp"

using spt::model::PropertiesModel;

int PropertiesModel::rowCount( const Wt::WModelIndex& ) const
{
  return 6 + static_cast<int>( std::distance( node->begin(), node->end() ) );
}

std::any PropertiesModel::data( const Wt::WModelIndex& index, Wt::ItemDataRole role ) const
{
  if ( role != Wt::ItemDataRole::Display
      && role != Wt::ItemDataRole::Edit
      && role != Wt::ItemDataRole::ToolTip ) return {};

  if ( index.row() >= 6 + rowCount ( index ) || index.column() >= 2 ) return {};
  if ( index.row() == 0 ) return index.column() == 0 ? "id" : node->id;
  if ( index.row() == 1 ) return index.column() == 0 ? "duration" : std::to_string( node->duration );
  if ( index.row() == 2 ) return index.column() == 0 ? "timestamp" : util::isoDateMicros( node->timestamp );
  if ( index.row() == 3 ) return index.column() == 0 ? "file" : node->file;
  if ( index.row() == 4 ) return index.column() == 0 ? "line" : std::to_string( node->line );
  if ( index.row() == 5 ) return index.column() == 0 ? "function" : node->function;
  auto it = node->begin();
  std::advance( it, index.row() - 6 );
  return index.column() == 0 ? it->first : it->second;
}

std::any PropertiesModel::headerData( int section, Wt::Orientation orientation, Wt::ItemDataRole role ) const
{
  if ( orientation != Wt::Orientation::Horizontal ) return WAbstractTableModel::headerData( section, orientation, role );

  if ( role == Wt::ItemDataRole::Link ) return WAbstractItemModel::headerData( section, orientation, role );

  if ( role != Wt::ItemDataRole::Display && role != Wt::ItemDataRole::Edit ) return {};

  return section == 0 ? "Key" : "Value";
}



