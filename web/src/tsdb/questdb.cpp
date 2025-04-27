//
// Created by Rakesh on 06/06/2023.
//

#include "pool.hpp"
#include "questdb.hpp"
#include "view/application.hpp"
#include "util/date.hpp"

#include <cstdlib>
#include <chrono>
#include <format>
#include <memory>
#include <set>
#include <boost/algorithm/string/replace.hpp>
#include <fmt/format.h>
#include <range/v3/algorithm/copy.hpp>
#include <pqxx/pqxx>

using std::operator""s;
using std::operator""sv;

namespace spt::tsdb::pquestdb
{
  struct ConnectionDeleter
  {
    void operator()( pqxx::connection* c )
    {
      c->close();
      delete c;
    }
  };

  struct Connection
  {
    Connection()
    {
      if ( const char* url = std::getenv( "QUESTDB_URI" ) )
      {
        connection.reset( new pqxx::connection( url ) );
      }
      else
      {
        connection.reset( new pqxx::connection( "postgresql://user:quest@localhost:8812/qdb"s ) );
      }
    }

    pqxx::nontransaction transaction()
    {
      return pqxx::nontransaction{ *connection.get() };
    }

    bool valid()
    {
      try
      {
        auto tx = transaction();
        auto res = tx.exec( "select now()"sv );
        return true;
      }
      catch( const std::exception& e )
      {
        apm::Application::instance()->log( "warn" ) << __FILE__ << " " << __func__ << " " << __LINE__ <<  " Error checking database connection. " << e.what();
      }

      return false;
    }

  private:
    std::unique_ptr<pqxx::connection, ConnectionDeleter> connection{ nullptr };
  };

  std::unique_ptr<Connection> create()
  {
    return std::make_unique<Connection>();
  }

  struct PoolHolder
  {
    static PoolHolder& instance()
    {
      static PoolHolder holder;
      return holder;
    }

    std::optional<pool::Pool<Connection>::Proxy> acquire()
    {
      auto proxy = pool.acquire();

      int retry = 0;
      while ( !(*proxy)->valid() )
      {
        proxy = pool.acquire();
        if ( ++retry > 10 )
        {
          apm::Application::instance()->log( "crit" ) << __FILE__ << " " << __func__ << " " << __LINE__ <<  " Unable to get valid connection to database";
          return std::nullopt;
        }
      }

      return proxy;
    }

  private:
    PoolHolder() = default;

    pool::Pool<Connection> pool{ create };
  };
}

std::expected<pqxx::result, std::string> spt::tsdb::execute( std::string_view query )
{
  using O = std::expected<pqxx::result, std::string>;

  try
  {
    auto proxy = pquestdb::PoolHolder::instance().acquire();
    if ( !proxy ) return O{ std::unexpect, "Unable to get connection to database" };

    auto tx = (*proxy)->transaction();
    return O{ std::in_place, tx.exec( query ) };
  }
  catch ( const pqxx::sql_error& e )
  {
    apm::Application::instance()->log( "warn" ) << __FILE__ << " " << __func__ << " " << __LINE__ << std::format( " SQL error executing: {}. {}", query, e.what() );
    return O{ std::unexpect, e.what() };
  }
  catch ( const std::exception& e )
  {
    apm::Application::instance()->log( "warn" ) << __FILE__ << " " << __func__ << " " << __LINE__ << std::format( " Exception executing query: {}. {}", query, e.what() );
    return O{ std::unexpect, e.what() };
  }
  catch ( ... )
  {
    apm::Application::instance()->log( "warn" ) << __FILE__ << " " << __func__ << " " << __LINE__ << std::format( " Unknown error executing: {}.", query );
  }

  return O{ std::unexpect, "Unknown error" };
}
