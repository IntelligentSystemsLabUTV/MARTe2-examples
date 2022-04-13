#!/bin/bash

export HOME=/home/marte2
export MDSPLUS_DIR=$HOME/mdsplus
export SDN_CORE_DIR=$HOME/SDN_1.0.12_nonCCS
if [[ "$(arch)" == "armv7l" || "$(arch)" == "armv8l" ]]; then
    export TARGET=armv8-linux
    export MARTe2_DIR=$HOME/MARTe2-armv8
elif [[ "$(arch)" == "x86_64" ]]; then
    export TARGET=x86-linux
    export MARTe2_DIR=$HOME/MARTe2
else
    echo "Error: $1 not supported. Architectures availables are armv8-linux and x86-linux"
    exit 1
fi
export MARTe2_Components_DIR=$HOME/MARTe2-components
export MARTe2_Examples_DIR=$HOME/workspace/MARTe2
export WRRD_path=$MARTe2_Examples_DIR/Trees/WrRd
export PUBSUBWR_path=$MARTe2_Examples_DIR/Trees/PubSubWr
