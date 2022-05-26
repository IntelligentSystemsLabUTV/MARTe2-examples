#!/bin/bash

export HOME=/home/marte2
if [[ "$(uname -m)" == "aarch64" ]]; then
    export TARGET=armv8-linux
    export MARTe2_DIR=$HOME/MARTe2-armv8
elif [[ "$(uname -m)" == "x86_64" ]]; then
    export TARGET=x86-linux
    export MARTe2_DIR=$HOME/MARTe2
else
    echo "Error: $(uname -m) not supported. Architectures availables are aarch64 and x86_64"
    exit 1
fi
export MARTe2_Components_DIR=$HOME/MARTe2-components
export MARTe2_Examples_DIR=$HOME/workspace/MARTe2
export MDSPLUS_DIR=$HOME/MDSplus
export SDN_CORE_DIR=$HOME/SDN_1.0.12_nonCCS

export control_path=$HOME/workspace/Trees/Control
export log_path=$HOME/workspace/Trees/Logger
export pubsub_path=$HOME/workspace/Trees/Logger
