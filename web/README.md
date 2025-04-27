# APM Viewer Web Application
A simple application built using [Wt](https://www.webtoolkit.eu/wt).  Similar to the **Desktop** application,
the primary purpose of this application is to display APM records as *forrests* that follow the function
call chain in REST API servers.

This application is intended to deploy in the same network as [QuestDb](https://questdb.com/).
Unlike the desktop application, this application connects to QuestDB over the Postgres Wire Protocol,
to retrieves data.
