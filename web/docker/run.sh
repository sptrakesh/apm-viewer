#!/bin/sh

Network()
{
  exists=`docker network ls | grep spt-net`
  if [ -z "$exists" ]
  then
    docker network create --driver bridge spt-net
  fi
}

Start()
{
  Network
  docker pull questdb/questdb
  docker run -d --rm \
    -v "$HOME/tmp/wirepulse/questdb:/var/lib/questdb" \
    -e QDB_PG_READONLY_USER_ENABLED=true \
    -p 9000:9000 -p 9009:9009 -p 8812:8812 -p 9003:9003 \
    --network spt-net \
    --name questdb questdb/questdb
}

Stop()
{
  docker stop questdb
}

StartStack()
{
  Start

  if [ ! -f $HOME/tmp/spt/apm-sessions.json ]
  then
    touch $HOME/tmp/spt/apm-sessions.json
  fi
  docker run --rm -d \
    -e "APM_USERNAME=apm" \
    -e "APM_PASSWORD=apm" \
    -e "QUESTDB_URI=postgresql://user:quest@questdb:8812/qdb" \
    -v "$HOME/tmp/spt/apm-sessions.json:/tmp/apm-sessions.json" \
    -p 8000:8000 \
    --network spt-net \
    --name apm-app sptrakesh/apm-app
}

StopStack()
{
  docker stop apm-app questdb
}

case "$1" in
  'start')
    Start
    ;;
  'stop')
    Stop
    ;;
  'start-stack')
    StartStack
    ;;
  'stop-stack')
    StopStack
    ;;
  *)
    echo "Usage: $0 <start|start-stack|stop|stop-stack>"
    exit 1
    ;;
esac
