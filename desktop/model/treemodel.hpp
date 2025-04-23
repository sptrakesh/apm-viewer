//
// Created by Rakesh on 21/04/2025.
//

#pragma once

#include "node.hpp"

#include <QtCore/QAbstractItemModel>

namespace model
{
  class TreeModel : public QAbstractItemModel
  {
    Q_OBJECT

  public:
    Q_DISABLE_COPY_MOVE(TreeModel)

    explicit TreeModel( Node* node, QObject *parent = nullptr ) : QAbstractItemModel( parent ), root( std::unique_ptr<Node>( node ) ) {}
    ~TreeModel() = default;

    [[nodiscard]] QVariant data( const QModelIndex& index, int role ) const override;
    Qt::ItemFlags flags( const QModelIndex& index ) const override;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
    QModelIndex index( int row, int column, const QModelIndex& parent ) const override;
    QModelIndex parent( const QModelIndex &index ) const override;
    int rowCount( const QModelIndex& parent ) const override;
    int columnCount( const QModelIndex& parent ) const override;

    const Node* nodeFor( const QModelIndex& index ) const;

  private:
    std::unique_ptr<Node> root;
  };
}