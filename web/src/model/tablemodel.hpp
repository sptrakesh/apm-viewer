//
// Created by Rakesh on 25/04/2025.
//

#pragma once

#include <pqxx/result>
#include <Wt/WAbstractTableModel.h>
#include <Wt/WModelIndex.h>

namespace spt::model
{
  struct TableModel : Wt::WAbstractTableModel
  {
    explicit TableModel( pqxx::result result );

    [[nodiscard]] int columnCount(const Wt::WModelIndex& parent) const override { return static_cast<int>( columns.size() ); }
    [[nodiscard]] int rowCount(const Wt::WModelIndex& parent) const override { return result.size(); }
    [[nodiscard]] std::any data(const Wt::WModelIndex& index, Wt::ItemDataRole role) const override;
    [[nodiscard]] std::any headerData(int section, Wt::Orientation orientation, Wt::ItemDataRole role) const override;

    void sort( int, Wt::SortOrder ) override {}

  private:
    pqxx::result result;
    std::vector<std::string> columns;
  };
}