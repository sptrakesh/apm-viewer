# APM Viewer Web Application
A simple application built using [Wt](https://www.webtoolkit.eu/wt).  Similar to the **Desktop** application,
the primary purpose of this application is to display APM records as *forrests* that follow the function
call chain in REST API servers.

This application is intended to deploy in the same network as [QuestDb](https://questdb.com/).
Unlike the desktop application, this application connects to QuestDB over the Postgres Wire Protocol,
to retrieves data.

## Features
The application provides two views:
* **List View** Display the parent APM records (generally those where the `type` column is `null`) in a table.
  The query used to retrieve the parent APM records is editable.  Change the query as appropriate to filter
  by  additional conditions, time range, etc.
  Double-click a row to display a *tree view* with all the *child process* records for the parent APM record.
* ** Tree View** Display a tree with all the *child process* records for the latest parent APM records.  Select
  rows in the *tree* to view all available data for the record.

## Configuration
The application has very minimal configuration options.  The following environment variables can be used to
control the aspects that are configurable.
* `APM_USERNAME` The `username` to use to log in to the application.  Defaults to `apmuser`.
* `APM_PASSWORD` The `password` to use to log in to the application.  See [authwidget](src/view/authwidget.cpp)
  for the default value.
* `QUESTDB_URI` The PostgreSQL connection URI.  Default `postgresql://user:quest@localhost:8812/qdb`
* `APM_TABLE` The database table that holds the APM records.  Defaults to `webapm`.  Only one table is supported
  by the application at present.