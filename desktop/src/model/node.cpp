//
// Created by Rakesh on 21/04/2025.
//

#include "node.hpp"
#include "../date/date.hpp"

#include <ranges>

using model::Node;

Node* Node::appendChild()
{
  children.emplace_back( std::make_unique<Node>( this ) );
  return children.back().get();
}

Node* Node::appendChild( const Node& node )
{
  const auto et = node.timestamp + std::chrono::duration_cast<std::chrono::microseconds>( std::chrono::nanoseconds( node.duration ) );

  for ( const auto& child : children )
  {
    const auto cet = child->timestamp + std::chrono::duration_cast<std::chrono::microseconds>( std::chrono::nanoseconds( child->duration ) );
    if ( et >= cet ) continue;
    return child->appendChild( node );
  }

  auto n = appendChild();
  n->id = node.id;
  n->duration = node.duration;
  n->timestamp = node.timestamp;
  n->file = node.file;
  n->line = node.line;
  n->function = node.function;
  return n;
}


void Node::add( const QString& key, const QString& value )
{
  map.insert( key, value );
}


Node* Node::child( int row )
{
  return row >= 0 && row < childCount() ? children.at(row).get() : nullptr;
}

int Node::row() const
{
  if ( parent == nullptr ) return 0;
  const auto it = std::ranges::find_if( parent->children,
    [this]( const std::unique_ptr<Node>& node ) { return node.get() == this; } );

  if ( it != std::ranges::end( parent->children ) ) return static_cast<int>( std::ranges::distance( parent->children.cbegin(), it ) );
  Q_ASSERT( false ); // should not happen
  return -1;
}

QVariant Node::data( int column ) const
{
  switch ( column )
  {
  case 0: return parent == nullptr ? QVariant::fromValue( QString( "Id" ) ) : id;
  case 1:
  {
    if ( parent == nullptr ) return QVariant::fromValue( QString( "Duration (μs)" ) );
    return duration == 0.0 ? QVariant{} : QVariant::fromValue( duration / 1000.0 );
  }
  case 2: return ( parent == nullptr ) ? QVariant::fromValue( QString( "Timestamp" ) ) : QString::fromStdString( date::isoDateMicros( timestamp ) );
  case 3: return ( parent == nullptr ) ? QVariant::fromValue( QString( "File" ) ) : file;
  case 4:
  {
    if ( parent == nullptr ) return QVariant::fromValue( QString( "Line" ) );
    return line == 0 ? QVariant{} : QVariant::fromValue( line );
  }
  case 5: return ( parent == nullptr ) ? QVariant::fromValue( QString( "Function" ) ) : function;
  default: return {};
  }
}

void Node::sort()
{
  std::ranges::sort( children, []( const std::unique_ptr<Node>& lhs, const std::unique_ptr<Node>& rhs ) { return lhs->timestamp < rhs->timestamp; } );
  for ( auto& child : children ) child->sort();
}
