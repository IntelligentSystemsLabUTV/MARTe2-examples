#!/bin/bash

# Arguments -l LOADER -f FILENAME (-m MESSAGE | -s STATE) [-d cgdb|strace]
# -l LOADER = The Loader to use
# -f FILENAME = MARTe configuration file
# -m MESSAGE = Start message
# -s STATE = RealTimeApplication first state
# -d cgdb = Run with cgdb
# -d strace = Run with strace

export log_path=$HOME/workspace/Trees/Logger

# Run with cgdb or strace?
DEBUG=""

# Consume input arguments
while [[ $# -gt 1 ]]
do
key="$1"

case $key in
    -l|--loader)
    LOADER="$2"
    shift # past argument
    ;;
    -f|--file)
    FILE="$2"
    shift # past argument
    ;;
    -m|--message)
    MESSAGE="$2"
    shift # past argument
    ;;
    -s|--state)
    STATE="$2"
    shift # past argument
    ;;
    -d|--debug)
    DEBUG="$2"
    shift # past argument
    ;;
    --default)
    DEFAULT=YES
    ;;
    *)
          # unknown option
    ;;
esac
shift # past argument or value
done

# MARTe2 Core
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_DIR/Build/$TARGET/Core/
# MARTe2 Components
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/DataSources/LinuxTimer/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/DataSources/LoggerDataSource/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/GAMs/IOGAM/
# MARTe2 Examples
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Examples_DIR/Build/$TARGET/Components/Interfaces/TCPSocketMessageProxy/
# MDSplus
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MDSPLUS_DIR/lib/

echo $LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH

MARTeAPP=$MARTe2_DIR/Build/$TARGET/App/MARTeApp.ex

#Start with cgdb or with strace
if [ "$DEBUG" = "cgdb" ]
then
    if [ -z ${STATE+x} ]; then
        cgdb --args $MARTeAPP -l $LOADER -f $FILE -m $MESSAGE
    else
        cgdb --args $MARTeAPP -l $LOADER -f $FILE -s $STATE
    fi
elif [ "$DEBUG" = "strace" ]
then
    if [ -z ${STATE+x} ]; then
        strace -o/tmp/strace.err $MARTeAPP -l $LOADER -f $FILE -m $MESSAGE
    else
        strace -o/tmp/strace.err $MARTeAPP -l $LOADER -f $FILE -s $STATE
    fi
else
    if [ -z ${STATE+x} ]; then
        $MARTeAPP -l $LOADER -f $FILE -m $MESSAGE
    else
        $MARTeAPP -l $LOADER -f $FILE -s $STATE
    fi
fi