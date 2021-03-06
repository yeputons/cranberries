#!/bin/bash

# This scripts runs Zookeeper server in a Docker container.
#
# Just `source zookeeper_cc/run_zookeeper_server.sh` to start Zookeeper
# server in background Docker container and get connection string in
# ZOOKEEPER_TEST_HOSTS. The container will be automatically shut down on exit.

source sh_utils/sh_utils.sh

if ! type docker >/dev/null 2>&1; then
  echo "Docker is required to run Zookeeper server" >&2
  exit 1
fi

echo -n "Starting dockered Zookeeper... "
CID=$(docker run -d zookeeper)
echo "OK, container id is $CID"

function stop_zookeeper() {
  echo -n "Stopping container $CID... "
  docker stop "$CID" >/dev/null
  echo "OK"
}
atexit stop_zookeeper

IP=$(docker inspect --format '{{ .NetworkSettings.IPAddress }}' "$CID")
PORT=2181
echo -n "Waiting for Zookeeper to start on $IP:$PORT..."
wait_cmd_with_timeout 5 nc -w 1 "$IP" "$PORT"
echo " OK"

export ZOOKEEPER_TEST_HOSTS="$IP:$PORT"
export ZOOKEEPER_TEST_DOCKER_CONTAINER_ID="$CID"
