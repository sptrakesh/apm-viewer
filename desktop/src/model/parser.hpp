//
// Created by Rakesh on 21/04/2025.
//

#pragma once

#include "treemodel.hpp"

namespace model
{
  class Parser : public QObject
  {
    Q_OBJECT

  public:
    explicit Parser( QString  csvFile, QObject* parent = nullptr ) : csvFile( std::move( csvFile ) ), parent( parent ) {}

  signals:
    void progress( std::size_t total );
    void finished( model::Node* root );

  public slots:
    void parse();

  private:
    Node* getChild( Node* root, std::string_view id );

    QString csvFile;
    QObject* parent;
  };
}