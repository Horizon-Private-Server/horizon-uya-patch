#!/bin/bash
set -e

# remove current and create new bin directory for horizon-uya-patch/
sudo rm -rf bin
mkdir bin

# run docker and run "container_commands.sh"
docker run -v "${PWD}":/src ps2dev/ps2dev:v1.2.0 /bin/sh -c "cd src; sh container_commands.sh"

# remove current and create new bin file in horizon-server/docker/medius_plugins/
sudo rm -rf ../horizon-server/docker/medius_plugins/bin
mkdir ../horizon-server/docker/medius_plugins/bin

# copy misc/ binaries and patch files over to horizon-server.
cp misc/* ../horizon-server/docker/medius_plugins/bin
cp bin/ ../horizon-server/docker/medius_plugins/bin/patch -R
