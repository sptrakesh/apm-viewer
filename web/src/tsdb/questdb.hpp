//
// Created by Rakesh on 06/06/2023.
//

#pragma once

#include <expected>
#include <string>
#include <string_view>
#include <vector>

#include <pqxx/result>

namespace spt::tsdb
{
  std::expected<pqxx::result, std::string> execute( std::string_view query );
}