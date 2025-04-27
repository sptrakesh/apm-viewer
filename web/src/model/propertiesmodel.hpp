//
// Created by Rakesh on 26/04/2025.
//

#pragma once

#include "node.hpp"
#include <Wt/WAbstractTableModel.h>

namespace spt::model
{
  struct PropertiesModel : Wt::WAbstractTableModel
  {
    explicit PropertiesModel( Node* node ) : node( node ) {}

    [[nodiscard]] int columnCount(const Wt::WModelIndex& parent) const override { return 2; }
    [[nodiscard]] int rowCount(const Wt::WModelIndex& parent) const override;
    [[nodiscard]] std::any data(const Wt::WModelIndex& index, Wt::ItemDataRole role) const override;
    [[nodiscard]] std::any headerData(int section, Wt::Orientation orientation, Wt::ItemDataRole role) const override;

  private:
    Node* node;
  };
}