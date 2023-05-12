#!/bin/bash
set -e

sudo rm -rf bin
mkdir bin

docker run -v "${PWD}":/src ps2dev/ps2dev:v1.2.0 /bin/sh -c "cd src; sh container_commands.sh"

cp bin/ ../horizon-server/docker/medius_plugins/bin -R
cp bin/ ../horizon-server/docker/medius_plugins/bin/patch -R

cp misc/* ../horizon-server/docker/medius_plugins/bin
cp misc/* ../horizon-server/docker/medius_plugins/bin/patch


