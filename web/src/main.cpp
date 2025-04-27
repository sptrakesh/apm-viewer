//
// Created by Rakesh on 19/09/2023.
//

#include "view/application.hpp"

#include <cstdlib>
#include <Wt/WServer.h>

int main( int argc, char** argv )
{
  using std::operator""sv;
  auto val = getenv( "CONSOLE" );
  bool console = val != nullptr && ( std::string_view{ val } == "true"sv );

  try
  {
    Wt::WServer server{ argc, argv, WTHTTP_CONFIGURATION };
    server.addEntryPoint( Wt::EntryPointType::Application, []( const Wt::WEnvironment& env )
    {
      return std::make_unique<spt::apm::Application>( env );
    } );

    server.run();
  }
  catch ( const Wt::WServer::Exception& e )
  {
    std::cerr << e.what() << '\n';
  }
  catch ( const std::exception& e )
  {
    std::cerr << "exception: " << e.what() << '\n';
  }
}