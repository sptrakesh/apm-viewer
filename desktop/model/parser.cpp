//
// Created by Rakesh on 21/04/2025.
//

#include "csv.hpp"
#include "parser.hpp"
#include "../date/date.hpp"

#include <span>

#include <QtCore/QEventLoop>
#include <QtCore/QLoggingCategory>

Q_LOGGING_CATEGORY( PARSER, "Parser" )

using model::Parser;

namespace
{
  namespace pparser
  {
    using namespace model;

    void parse( std::string_view v, Node* node )
    {
      auto value =  QString::fromUtf8( v );
      value.replace( ' ', 'T' );
      if ( !value.endsWith( 'Z' ) ) value.append( 'Z' );

      auto res = date::parseISO8601( value.toStdString() );
      if ( res.has_value() ) node->timestamp = res.value();
      else qWarning(PARSER) << "Failed to parse timestamp: " << value;
    }

    void parse( Node* node, const csv::CSVRow& row, std::span<const std::string> columns )
    {
      for ( const auto& column : columns )
      {
        if ( column == "id" ) continue;
        if ( row[column].is_null() ) continue;

        if ( column == "file" ) node->file = QString::fromUtf8( row[column].get_sv() );
        else if ( column == "line" ) node->line = row[column].get<int>();
        else if ( column == "function" ) node->function = QString::fromUtf8( row[column].get_sv() );
        else if ( column == "timestamp" || column == "timestamp_iso" ) parse( row[column].get_sv(), node );
        else if ( column == "duration" ) node->duration = row[column].get<int64_t>();

        node->add( QString::fromStdString( column ), QString::fromUtf8( row[column].get_sv() ) );
      }
    }

    Node parse( const csv::CSVRow& row, std::span<const std::string> columns )
    {
      auto node = Node{};

      for ( const auto& column : columns )
      {
        if ( column == "id" ) continue;
        if ( row[column].is_null() ) continue;

        if ( column == "file" ) node.file = QString::fromUtf8( row[column].get_sv() );
        else if ( column == "line" ) node.line = row[column].get<int>();
        else if ( column == "function" ) node.function = QString::fromUtf8( row[column].get_sv() );
        else if ( column == "timestamp" || column == "timestamp_iso" ) parse( row[column].get_sv(), &node );
        else if ( column == "duration" ) node.duration = row[column].get<int64_t>();
      }

      return node;
    }
  }
}

void Parser::parse()
{
  QEventLoop loop;
  auto root = new Node();

  auto reader = csv::CSVReader( csvFile.toStdString() );
  auto first = true;
  auto columns = std::vector<std::string>{};

  for ( const auto& row : reader )
  {
    if ( first )
    {
      columns = row.get_col_names();
      first = false;
      continue;
    }

    auto id = row["id"].get_sv();
    Node* node = getChild( root, id );
    if ( node == nullptr )
    {
      node = root->appendChild();
      node->id = QString::fromUtf8( id );
    }
    else
    {
      auto temp = pparser::parse( row, columns );
      temp.id = QString::fromUtf8( id );
      node = node->appendChild( temp );
    }

    pparser::parse( node, row, columns );
    if ( reader.n_rows() % 100 == 0 ) emit progress( reader.n_rows() );
  }

  emit progress( reader.n_rows() );
  root->sort();

  emit finished( root );
}

model::Node* Parser::getChild( Node* root, std::string_view id )
{
  for ( int i = 0; i < root->childCount(); ++i )
  {
    auto* node = root->child( i );
    if ( node->id == id ) return node;
  }

  return nullptr;
}