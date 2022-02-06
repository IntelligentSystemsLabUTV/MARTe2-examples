#!/bin/bash

if [[ "$#" -ne 2 ]]; then
    echo "Usage: ./docker_run <arch> <image>"
    exit 1
fi

arch=""
if [[ "$(arch)" == "armv7l" || "$(arch)" == "armv8l" ]]; then
    arch="armv8-linux"
elif [[ "$(arch)" == "x86_64" ]]; then
    arch="x86-linux"
else
    echo "Error: $1 not supported. Architecturs availables armv8-linux and x86-linux"
    echo "Usage: ./docker_run <arch> <image>"
    exit 1
fi

sudo docker run -it --privileged --network host \
--env TERM=xterm-256color \
--env DISPLAY=$DISPLAY \
--user marte \
-v ~/.ssh:/home/marte/.ssh:ro \
-v /home/pi/MARTe2-examples/config/zsh_history:/home/marte/zsh_history \
-v /home/pi/MARTe2-examples/config/.aliases.sh:/home/marte/.aliases.sh \
-v /home/pi/MARTe2-examples/config/.bashrc:/home/marte/.bashrc \
-v /home/pi/MARTe2-examples/config/.nanorc:/home/marte/.nanorc \
-v /home/pi/MARTe2-examples/config/.zshrc:/home/marte/.zshrc \
-v /home/pi/MARTe2-examples/config/$arch/.p10k.zsh:/home/marte/.p10k.zsh \
-v /home/pi/MARTe2-examples:/home/marte/workspace \
$2 zsh
