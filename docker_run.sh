#!/bin/bash

if [[ "$#" -ne 1 ]]; then
    echo "Usage: ./docker_run <image>"
    exit 1
fi

sudo docker run -it --privileged --network host \
--env TERM=xterm-256color \
--env DISPLAY=$DISPLAY \
--user marte \
-v ~/.ssh:/home/marte/.ssh:ro \
-v /home/pi/MARTe2-examples/config-armv8/zsh_history:/home/marte/zsh_history \
-v /home/pi/MARTe2-examples/config-armv8/.aliases.sh:/home/marte/.aliases.sh \
-v /home/pi/MARTe2-examples/config-armv8/.bashrc:/home/marte/.bashrc \
-v /home/pi/MARTe2-examples/config-armv8/.nanorc:/home/marte/.nanorc \
-v /home/pi/MARTe2-examples/config-armv8/.p10k.zsh:/home/marte/.p10k.zsh \
-v /home/pi/MARTe2-examples/config-armv8/.zshrc:/home/marte/.zshrc \
-v /home/pi/MARTe2-examples:/home/marte/workspace \
$1 zsh
