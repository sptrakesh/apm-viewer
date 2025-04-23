# Desktop APM Viewer
Simple viewer application built using [Qt](https://qt.io/).  Renders a forrest view of APM records exported as
CSV from database.  We generally export the CSV from our Grafana dashboard and view the function call chain.

We use the following query from our Grafana panel to select the latest APM records to export and then view
in the viewer.
```sql
with latest as 
(
  select id as apm_id
  from webapm 
  where type is null
  and action in ($action)
  and entity in ($entity)
  and application in ($application)
  order by timestamp desc
  limit $limit
)
select webapm.*, to_str(timestamp, 'yyyy-MM-ddTHH:mm:ss.U+') as 'timestamp_iso'
from webapm
inner join latest on webapm.id = latest.apm_id
order by timestamp
```