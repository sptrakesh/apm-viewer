//
// Created by Rakesh on 26/04/2025.
//

#pragma once

#include "node.hpp"

#include <pqxx/result>
#include <Wt/WAbstractItemModel.h>
#include <Wt/WModelIndex.h>

namespace spt::model
{
  struct TreeModel : Wt::WAbstractItemModel
  {
    explicit TreeModel( const pqxx::result& result );

    [[nodiscard]] int columnCount( const Wt::WModelIndex& parent ) const override;
    [[nodiscard]] int rowCount( const Wt::WModelIndex& parent ) const override;
    [[nodiscard]] Wt::WModelIndex parent( const Wt::WModelIndex& index ) const override;
    [[nodiscard]] std::any data( const Wt::WModelIndex& index, Wt::ItemDataRole role ) const override;
    [[nodiscard]] std::any headerData( int section, Wt::Orientation orientation, Wt::ItemDataRole role ) const override;
    [[nodiscard]] Wt::WModelIndex index( int row, int column, const Wt::WModelIndex& parent ) const override;

    [[nodiscard]] const Node* nodeFor( const Wt::WModelIndex& index ) const;

  private:
    std::unique_ptr<Node> root;
  };
}