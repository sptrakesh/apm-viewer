//
// Created by Rakesh on 26/04/2025.
//

#include "propertiesmodel.hpp"

using spt::model::PropertiesModel;

int PropertiesModel::rowCount( const Wt::WModelIndex& ) const
{
  return static_cast<int>( std::distance( node->begin(), node->end() ) );
}

std::any PropertiesModel::data( const Wt::WModelIndex& index, Wt::ItemDataRole role ) const
{
  if ( role != Wt::ItemDataRole::Display
      && role != Wt::ItemDataRole::Edit
      && role != Wt::ItemDataRole::ToolTip ) return {};

  if ( index.row() >= rowCount ( index ) || index.column() >= 2 ) return {};
  auto it = node->begin();
  std::advance( it, index.row() );
  return index.column() == 0 ? it->first : it->second;
}

std::any PropertiesModel::headerData( int section, Wt::Orientation orientation, Wt::ItemDataRole role ) const
{
  if ( orientation != Wt::Orientation::Horizontal ) return WAbstractTableModel::headerData( section, orientation, role );

  if ( role == Wt::ItemDataRole::Link ) return WAbstractItemModel::headerData( section, orientation, role );

  if ( role != Wt::ItemDataRole::Display && role != Wt::ItemDataRole::Edit ) return {};

  return section == 0 ? "Key" : "Value";
}



