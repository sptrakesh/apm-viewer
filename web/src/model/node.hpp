//
// Created by Rakesh on 21/04/2025.
//

#pragma once

#include <memory>

#include <any>
#include <chrono>
#include <map>
#include <vector>

namespace spt::model
{
  struct Node
  {
    using Properties =  std::map<std::string, std::string, std::less<>>;
    explicit Node( Node* parent = nullptr ) : parent( parent ) {}

    Node* appendChild();
    Node* appendChild( const Node& node );
    void add( const std::string& key, const std::string& value );

    Node* child( int row );
    [[nodiscard]] int row() const;
    [[nodiscard]] std::any data( int column ) const;

    [[nodiscard]] int childCount() const { return static_cast<int>( children.size() ); }
    [[nodiscard]] int columnCount() const { return 6; }
    Node* parentNode() { return parent; }

    [[nodiscard]] Properties::const_iterator begin() const { return properties.begin(); }
    [[nodiscard]] Properties::const_iterator end() const { return properties.end(); }

    void sort();

    std::string id;
    std::string file;
    std::string function;
    std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> timestamp;
    int64_t duration{ 0 };
    int line{ 0 };

  private:
    std::vector<std::unique_ptr<Node>> children;
    Properties properties;
    Node* parent;
  };
}