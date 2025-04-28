#!/bin/sh

DIR=`dirname $0`
. $DIR/env.sh

(cd $DIR/..;
  docker build --compress --force-rm -f docker/Dockerfile -t sptrakesh/$NAME:$VERSION -t sptrakesh/$NAME:latest .)