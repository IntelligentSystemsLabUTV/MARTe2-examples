#!/bin/bash

# Development environment for MARTe2 on x86
#
# Alessandro Tenaglia <alessandro.tenaglia42@gmail.com>
# Federico Oliva <lvofrc95@outlook.it>
#
# January 6, 2022

if [[ "$#" -ne 1 ]]; then
    echo "Usage: ./docker_run <image>"
    exit 1
fi

arch=""
if [[ "$(arch)" == "armv7l" || "$(arch)" == "armv8l" ]]; then
    arch="armv8-linux"
elif [[ "$(arch)" == "x86_64" ]]; then
    arch="x86-linux"
else
    echo "Error: $1 not supported. Architectures availables are armv8-linux and x86-linux"
    echo "Usage: ./docker_run <arch> <image>"
    exit 1
fi

sudo docker run -it --privileged --network host \
--env TERM=xterm-256color \
--env DISPLAY=$DISPLAY \
--user marte \
-v ~/.ssh:/home/marte/.ssh:ro \
-v $(pwd)/config/zsh_history:/home/marte/zsh_history \
-v $(pwd)/config/.aliases.sh:/home/marte/.aliases.sh \
-v $(pwd)/config/.bashrc:/home/marte/.bashrc \
-v $(pwd)/config/.nanorc:/home/marte/.nanorc \
-v $(pwd)/config/.zshrc:/home/marte/.zshrc \
-v $(pwd)/config/$arch/.p10k.zsh:/home/marte/.p10k.zsh \
-v $(pwd)/:/home/marte/workspace \
$1 zsh
