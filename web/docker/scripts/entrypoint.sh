#!/bin/sh

PIDFILE='/tmp/service.pid'
LOGFILE='/tmp/restart.log'

Defaults()
{
  if [ -z "$PORT" ]
  then
    PORT=8000
    echo "PORT not set.  Will default to $PORT"
  fi
}

Required()
{
  if [ -z "$QUESTDB_URI" ]
  then
    echo "QUESTDB_URI not defined."
    exit 1
  fi
}

Service()
{
  echo "`date` Starting up APM viewer application on port $PORT" >> $LOGFILE
  /opt/spt/bin/apm-app --docroot /opt/spt/var/docroot \
    --approot /opt/spt/var/approot \
    --http-port $PORT --http-address 0.0.0.0 \
    --resources-dir /opt/local/share/Wt/resources \
    --deploy-path /a \
    -c /opt/spt/var/wt_config.xml \
    --pid-file $PIDFILE
  echo $! > $PIDFILE
}

Loop()
{
  while true
  do
    Service
  done
}

Required && Defaults && Loop