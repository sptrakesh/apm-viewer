//
// Created by Rakesh on 21/04/2025.
//

#pragma once

#include <memory>
#include <QtCore/QMap>
#include <QtCore/QVariant>

#include <chrono>
#include <vector>

namespace model
{
  class Node
  {
  public:
    explicit Node( Node* parent = nullptr ) : parent( parent ) {}

    Node* appendChild();
    Node* appendChild( const Node& node );
    void add( const QString& key, const QString& value );

    Node* child( int row );
    [[nodiscard]] int row() const;
    [[nodiscard]] QVariant data( int column ) const;

    [[nodiscard]] int childCount() const { return static_cast<int>( children.size() ); }
    [[nodiscard]] int columnCount() const { return 6; }
    Node* parentNode() { return parent; }

    [[nodiscard]] auto keyValues() const { return map.asKeyValueRange(); }

    void sort();

    QString id;
    QString file;
    QString function;
    std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> timestamp;
    int64_t duration{ 0 };
    int line{ 0 };

  private:
    std::vector<std::unique_ptr<Node>> children;
    QMap<QString, QString> map;
    Node* parent;
  };
}