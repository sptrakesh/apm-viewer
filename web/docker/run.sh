#!/bin/sh

Start()
{
  docker pull questdb/questdb
  docker run -d --rm \
    -v "$HOME/tmp/wirepulse/questdb:/var/lib/questdb" \
    -e QDB_PG_READONLY_USER_ENABLED=true \
    -p 9000:9000 -p 9009:9009 -p 8812:8812 -p 9003:9003 \
    --name questdb questdb/questdb
}

Stop()
{
  docker stop questdb
}

case "$1" in
  'start')
    Start
    ;;
  'stop')
    Stop
    ;;
  *)
    echo "Usage: $0 <start|stop>"
    exit 1
    ;;
esac
