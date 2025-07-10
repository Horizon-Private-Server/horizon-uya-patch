# remove current and create new bin directory for horizon-uya-patch/
Remove-Item -Recurse -Force ./bin
mkdir ./bin

# run docker and run "container_commands.sh"
docker run -v ${PWD}:/src ps2dev/ps2dev:v1.2.0 /bin/sh -c "cd src; sh container_commands.sh"

