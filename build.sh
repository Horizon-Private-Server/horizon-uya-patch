docker run -v "${PWD}":/src ps2dev/ps2dev:v1.2.0 /bin/sh -c "cd src; sh container_commands.sh"

cp bin/* ../horizon-server/docker/patch/

