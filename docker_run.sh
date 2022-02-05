#!/bin/bash

if [[ "$#" -ne 1 ]]; then
    echo "Usage: docker_run <image>"
    exit 1
fi

docker run -it --priveged --network host \
--env TERM=xterm-256color \
--env DISPLAY=$DISPLAY \
--user marte \
-v ~/.ssh:/home/$USERNAME/.ssh:ro \
-v ./config-armv8/zsh_history:/home/marte/zsh_history \
-v ./config-armv8/.aliases.sh:/home/marte/.aliases.sh \
-v ./config-armv8/.bashrc:/home/marte/.bashrc \
-v ./config-armv8/.nanorc:/home/marte/.nanorc \
-v ./config-armv8/.p10k.zsh:/home/marte/.p10k.zsh \
-v ./config-armv8/.zshrc:/home/marte/.zshrc \
-v ./:/home/marte/workspace \
$1 zsh
