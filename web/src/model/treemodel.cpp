//
// Created by Rakesh on 26/04/2025.
//

#include "node.hpp"
#include "treemodel.hpp"
#include "tsdb/pgtypes.hpp"

#include <Wt/WDateTime.h>
#include <boost/algorithm/string/replace.hpp>

#include "view/application.hpp"

using spt::model::TreeModel;

namespace
{
  namespace ptm
  {
    spt::model::Node* getChild( spt::model::Node* root, std::string_view id )
    {
      for ( int i = 0; i < root->childCount(); ++i )
      {
        auto* node = root->child( i );
        if ( node->id == id ) return node;
      }

      return nullptr;
    }

    void parseBasic( spt::model::Node& node, const pqxx::row& row )
    {
      for ( const auto& column : row )
      {
        if ( column.is_null() ) continue;
        auto name = std::string{ column.name() };
        boost::algorithm::replace_all( name, "\"", "" );
        if ( name == "id" ) node.id = column.as<std::string>();
        else if ( name == "file" ) node.file = column.as<std::string>();
        else if ( name == "line" ) node.line = column.as<int>();
        else if ( name == "function" ) node.function = column.as<std::string>();
        else if ( name == "duration" ) node.duration = column.as<int64_t>();
        else if ( name == "timestamp" )
        {
          auto ts = spt::tsdb::timestamp( column );
          if ( !ts.has_value() ) spt::apm::Application::instance()->log( "error" ) << __FILE__ << " " << __func__ << " " << __LINE__ << std::format( " Invalid timestamp {}", column.view() );
          node.timestamp = ts.value_or( spt::util::DateTime{} );
        }
      }
    }

    spt::model::Node parse( const pqxx::row& row )
    {
      auto node = spt::model::Node{};
      parseBasic( node, row );
      return node;
    }

    void parse( spt::model::Node* node, const pqxx::row& row )
    {
      using std::operator""s;
      static const auto ignore = std::array{ "id"s, "file"s, "line"s, "function"s, "duration"s, "timestamp"s };
      parseBasic( *node, row );

      for ( const auto& column : row )
      {
        if ( column.is_null() ) continue;

        auto name = std::string{ column.name() };
        boost::algorithm::replace_all( name, "\"", "" );
        if ( auto iter = std::ranges::find( ignore, name ); iter == std::ranges::end( ignore ) )
        {
          if ( spt::tsdb::isTimestamp( column ) )
          {
            if ( const auto ts = spt::tsdb::timestamp( column ); ts ) node->add( name, spt::util::isoDateMicros( *ts ) );
          }
          else if ( spt::tsdb::isString( column ) )
          {
            if ( const auto v = spt::tsdb::string( column ); v ) node->add( name, *v );
          }
          else if ( spt::tsdb::isNumber( column ) )
          {
            if ( const auto v = spt::tsdb::number( column ); v ) node->add( name, std::to_string( *v ) );
          }
        }
      }
    }
  }
}

TreeModel::TreeModel( const pqxx::result& result )
{
  root = std::make_unique<Node>();

  for ( const auto& row : result )
  {
    auto field = row.at( "id" );
    auto str = tsdb::string( field );
    if ( !str ) throw std::runtime_error( "Invalid node id" );

    Node* node = ptm::getChild( root.get(), *str );
    if ( node == nullptr )
    {
      node = root->appendChild();
      node->id = *str;
    }
    else
    {
      node = node->appendChild( ptm::parse( row ) );
    }

    ptm::parse( node, row );
  }

  root->sort();
}

int TreeModel::columnCount( const Wt::WModelIndex& parent ) const
{
  if ( parent.isValid() ) return static_cast<Node*>( parent.internalPointer() )->columnCount();
  return root->columnCount();
}

int TreeModel::rowCount( const Wt::WModelIndex& parent ) const
{
  if ( parent.column() > 0 ) return 0;

  const auto* pnode = parent.isValid() ? static_cast<const Node*>( parent.internalPointer() ) : root.get();
  return pnode->childCount();
}

Wt::WModelIndex TreeModel::parent( const Wt::WModelIndex& index ) const
{
  if ( !index.isValid() ) return {};

  auto* cnode = static_cast<Node*>( index.internalPointer() );
  auto* pnode = cnode->parentNode();

  return pnode != root.get() ? createIndex( pnode->row(), 0, pnode ) : Wt::WModelIndex{};
}

std::any TreeModel::data( const Wt::WModelIndex& index, Wt::ItemDataRole role ) const
{
  if ( role != Wt::ItemDataRole::Display
      && role != Wt::ItemDataRole::Edit
      && role != Wt::ItemDataRole::ToolTip ) return {};

  const auto* item = static_cast<const Node*>( index.internalPointer() );
  return item->data( index.column() );
}

std::any TreeModel::headerData( int section, Wt::Orientation orientation, Wt::ItemDataRole role ) const
{
  return orientation == Wt::Orientation::Horizontal && role == Wt::ItemDataRole::Display ? root->data( section ) : std::any{};
}

Wt::WModelIndex TreeModel::index( int row, int column, const Wt::WModelIndex& parent ) const
{
  if ( !hasIndex( row, column, parent ) ) return {};

  Node* node = parent.isValid() ?
    static_cast<Node*>( parent.internalPointer() ) : root.get();
  if ( auto* child = node->child( row ) ) return createIndex( row, column, child );
  return {};
}

const spt::model::Node* TreeModel::nodeFor( const Wt::WModelIndex& index ) const
{
  if ( !index.isValid() ) return nullptr;
  return static_cast<const Node*>( index.internalPointer() );
}