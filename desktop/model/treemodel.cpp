//
// Created by Rakesh on 21/04/2025.
//

#include "treemodel.hpp"

using model::TreeModel;

QModelIndex TreeModel::index( int row, int column, const QModelIndex& parent ) const
{
  if ( !hasIndex( row, column, parent ) ) return {};

  Node* node = parent.isValid() ?
    static_cast<Node*>( parent.internalPointer() ) : root.get();
  if ( const auto* child = node->child( row ) ) return createIndex( row, column, child );
  return {};
}

QModelIndex TreeModel::parent( const QModelIndex& index ) const
{
  if ( !index.isValid() ) return {};

  auto* cnode = static_cast<Node*>( index.internalPointer() );
  auto* pnode = cnode->parentNode();

  return pnode != root.get() ? createIndex( pnode->row(), 0, pnode ) : QModelIndex{};
}

int TreeModel::rowCount( const QModelIndex& parent ) const
{
  if ( parent.column() > 0 ) return 0;

  const auto* pnode = parent.isValid() ? static_cast<const Node*>( parent.internalPointer() ) : root.get();
  return pnode->childCount();
}

int TreeModel::columnCount( const QModelIndex& parent ) const
{
  if ( parent.isValid() ) return static_cast<Node*>( parent.internalPointer() )->columnCount();
  return root->columnCount();
}

QVariant TreeModel::data( const QModelIndex& index, int role ) const
{
  if ( !index.isValid() || role != Qt::DisplayRole ) return {};

  const auto* item = static_cast<const Node*>( index.internalPointer() );
  return item->data( index.column() );
}

Qt::ItemFlags TreeModel::flags( const QModelIndex& index ) const
{
  return index.isValid() ? QAbstractItemModel::flags( index ) : Qt::ItemFlags( Qt::NoItemFlags );
}

QVariant TreeModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
  return orientation == Qt::Horizontal && role == Qt::DisplayRole ? root->data( section ) : QVariant{};
}

const model::Node* TreeModel::nodeFor( const QModelIndex& index ) const
{
  if ( !index.isValid() ) return nullptr;
  return static_cast<const Node*>( index.internalPointer() );
}